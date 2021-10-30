// Copyright (C) 2021, Alex Edin <lutfisk@lutfisk.net>
// SPDX-License-Identifier: GPL-2.0+

#include "focus.h"
#include "editor.h"
#include "clr.h"
#include "chartypes.h"
#include "algo.h"
#include "draw.h"

#include <stdio.h>
#include <stdlib.h>

focus_t focus_find_local = { draw_find_local, NULL, input_find_local };

static char input_buf[512];
static lstr_t input = LSTR(input_buf, 0);

static usz start_y, start_x;

static isz selected_index = -1;
static usz result_count = 0;
static doc_pos_t* results = NULL;

void find_local(isz start_y_, isz start_x_) {
	input.len = 0;
	focus = focus_find_local;
	start_y = start_y_;
	start_x = start_x_;
}

void draw_find_local(global_t* ed_global, void* args) {
	rec_goto(2, lt_term_height - 1);
	rec_clearline(clr_strs[CLR_LIST_HEAD]);
	rec_lstr(input.str, input.len);

	rec_goto(2, lt_term_height);
	rec_clearline(clr_strs[CLR_LIST_HIGHL]);
	char buf[128];
	sprintf(buf, "Result %zu/%zu", selected_index + 1, result_count);
	rec_str(buf);
}

static
isz find_index(void) {
	for (usz i = 0; i < result_count; ++i) {
		doc_pos_t result = results[i];
		if (result.y > start_y || (result.y == start_y && result.x >= start_x))
			return i;
	}

	for (usz i = 0; i < result_count; ++i) {
		doc_pos_t result = results[i];
		if (result.y <= start_y || (result.y == start_y && result.x < start_x))
			return i;
	}
	return -1;
}

static
void update_results(editor_t* ed) {
	usz new_count = doc_find_str(&ed->doc, input, NULL);
	if (new_count) {
		if (new_count > result_count) {
			results = realloc(results, new_count * sizeof(doc_pos_t));
			if (!results)
				ferrf("Memory allocation failed: %s\n", os_err_str());
		}
		doc_find_str(&ed->doc, input, results);
	}
	result_count = new_count;

	selected_index = find_index();
}

void input_find_local(global_t* ed_global, u32 c) {
	editor_t* ed = *ed_global->ed;

	switch (c) {
	case LT_TERM_KEY_BSPACE:
		if (!input.len)
			edit_file(ed_global, ed);
		else {
			--input.len;
			update_results(ed);
		}
		break;

	case LT_TERM_KEY_BSPACE | LT_TERM_MOD_CTRL:
		if (!input.len) case LT_TERM_KEY_ESC:
			edit_file(ed_global, ed);
		else {
			input.len = 0;
			update_results(ed);
		}
		break;

	case LT_TERM_KEY_UP:
		if (selected_index > 0)
			--selected_index;
		else if (selected_index == 0)
			selected_index = max(result_count - 1, 0);
		break;

	case LT_TERM_KEY_DOWN:
		if (selected_index != -1 && ++selected_index >= result_count)
			selected_index = 0;
		break;

	case '\n':
		edit_file(ed_global, ed);
		ed_sync_selection(ed);
		return;

	default:
		if (c >= 32 && c < 127 && input.len < sizeof(input_buf)) {
			input.str[input.len++] = c;
			update_results(ed);
		}
		break;
	}

	if (selected_index >= 0) {
		doc_pos_t result = results[selected_index];
		ed_goto_line(ed, result.y);
		ed->cx = result.x;
		ed->sel_y = result.y;
		ed->sel_x = result.x + input.len;
	}
	else {
		ed_goto_line(ed, start_y);
		ed->cx = start_x;
		ed_sync_selection(ed);
	}
}


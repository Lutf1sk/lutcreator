// Copyright (C) 2021, Alex Edin <lutfisk@lutfisk.net>
// SPDX-License-Identifier: GPL-2.0+

#include "draw.h"
#include "clr.h"

#include <lt/io.h>
#include <lt/texted.h>

#include <string.h>

char* write_buf;
char* write_it;

void rec_str(char* str) {
	usz len = strlen(str);
	memcpy(write_it, str, len);
	write_it += len;
}

void rec_lstr(char* str, usz len) {
	memcpy(write_it, str, len);
	write_it += len;
}

void rec_c(char c) {
	*write_it++ = c;
}

void rec_nc(usz n, char c) {
	memset(write_it, c, n);
	write_it += n;
}


void rec_goto(u32 x, u32 y) {
	write_it += lt_sprintf(write_it, "\x1B[%ud;%udH", y, x);
}


void rec_clear(char* clr) {
	rec_str(clr);
	rec_str("\x1B[2J");
}

void rec_clearline(char* clr) {
	rec_str(clr);
	rec_str("\x1B[2K");
}

void rec_led(lt_texted_t* ed, char* sel_clr, char* normal_clr) {
	usz x1, x2;
	lt_texted_get_selection(ed, &x1, NULL, &x2, NULL);
	lstr_t str = lt_texted_line_str(ed, 0);

	rec_str(normal_clr);
	rec_lstr(str.str, x1);
	if (x1 == ed->cursor_x)
		rec_csave();
	rec_str(sel_clr);
	rec_lstr(str.str + x1, x2 - x1);
	if (x2 == ed->cursor_x)
		rec_csave();
	rec_str(normal_clr);
	rec_lstr(str.str + x2, str.len - x2);
}

void rec_csave(void) {
	rec_str("\x1B[s");
}

void rec_crestore(void) {
	rec_str("\x1B[u");
}


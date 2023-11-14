#include <lt/ctype.h>
#include <lt/str.h>
#include <lt/mem.h>

#include "highlight.h"
#include "doc.h"

typedef
enum multiline_mode {
	MLMODE_NONE,
	MLMODE_COMMENT,
} multiline_mode_t;

static
b8 is_keyword(lstr_t str) {
	if (!str.len)
		return 0;

	switch (str.str[0]) {
	case 'a':
		if (lt_lseq(str, CLSTR("abstract"))) return 1;
		if (lt_lseq(str, CLSTR("as"))) return 1;

		if (lt_lseq(str, CLSTR("async"))) return 1;
		if (lt_lseq(str, CLSTR("await"))) return 1;
		break;

	case 'b':
		if (lt_lseq(str, CLSTR("base"))) return 1;
		if (lt_lseq(str, CLSTR("break"))) return 1;
		break;

	case 'c':
		if (lt_lseq(str, CLSTR("case"))) return 1;
		if (lt_lseq(str, CLSTR("catch"))) return 1;
		if (lt_lseq(str, CLSTR("checked"))) return 1;
		if (lt_lseq(str, CLSTR("class"))) return 1;
		if (lt_lseq(str, CLSTR("const"))) return 1;
		if (lt_lseq(str, CLSTR("continue"))) return 1;
		break;

	case 'd':
		if (lt_lseq(str, CLSTR("default"))) return 1;
		if (lt_lseq(str, CLSTR("delegate"))) return 1;
		if (lt_lseq(str, CLSTR("do"))) return 1;
		break;

	case 'e':
		if (lt_lseq(str, CLSTR("else"))) return 1;
		if (lt_lseq(str, CLSTR("enum"))) return 1;
		if (lt_lseq(str, CLSTR("event"))) return 1;
		if (lt_lseq(str, CLSTR("explicit"))) return 1;
		if (lt_lseq(str, CLSTR("extern"))) return 1;
		break;

	case 'f':
		if (lt_lseq(str, CLSTR("false"))) return 1;
		if (lt_lseq(str, CLSTR("finally"))) return 1;
		if (lt_lseq(str, CLSTR("fixed"))) return 1;
		if (lt_lseq(str, CLSTR("for"))) return 1;
		if (lt_lseq(str, CLSTR("foreach"))) return 1;

		if (lt_lseq(str, CLSTR("from"))) return 1;
		break;

	case 'g':
		if (lt_lseq(str, CLSTR("goto"))) return 1;

		if (lt_lseq(str, CLSTR("get"))) return 1;
		break;

	case 'i':
		if (lt_lseq(str, CLSTR("if"))) return 1;
		if (lt_lseq(str, CLSTR("implicit"))) return 1;
		if (lt_lseq(str, CLSTR("in"))) return 1;
		if (lt_lseq(str, CLSTR("interface"))) return 1;
		if (lt_lseq(str, CLSTR("internal"))) return 1;
		if (lt_lseq(str, CLSTR("is"))) return 1;
		break;

	case 'l':
		if (lt_lseq(str, CLSTR("lock"))) return 1;

		if (lt_lseq(str, CLSTR("let"))) return 1;
		break;

	case 'n':
		if (lt_lseq(str, CLSTR("namespace"))) return 1;
		if (lt_lseq(str, CLSTR("new"))) return 1;
		if (lt_lseq(str, CLSTR("null"))) return 1;

		if (lt_lseq(str, CLSTR("nameof"))) return 1;
		break;

	case 'o':
		if (lt_lseq(str, CLSTR("operator"))) return 1;
		if (lt_lseq(str, CLSTR("out"))) return 1;
		if (lt_lseq(str, CLSTR("override"))) return 1;
		break;

	case 'p':
		if (lt_lseq(str, CLSTR("params"))) return 1;
		if (lt_lseq(str, CLSTR("private"))) return 1;
		if (lt_lseq(str, CLSTR("protected"))) return 1;
		if (lt_lseq(str, CLSTR("public"))) return 1;
		break;

	case 'r':
		if (lt_lseq(str, CLSTR("readonly"))) return 1;
		if (lt_lseq(str, CLSTR("ref"))) return 1;
		if (lt_lseq(str, CLSTR("return"))) return 1;
		break;

	case 's':
		if (lt_lseq(str, CLSTR("sealed"))) return 1;
		if (lt_lseq(str, CLSTR("sizeof"))) return 1;
		if (lt_lseq(str, CLSTR("stackalloc"))) return 1;
		if (lt_lseq(str, CLSTR("static"))) return 1;
		if (lt_lseq(str, CLSTR("struct"))) return 1;
		if (lt_lseq(str, CLSTR("switch"))) return 1;

		if (lt_lseq(str, CLSTR("set"))) return 1;
		break;

	case 't':
		if (lt_lseq(str, CLSTR("this"))) return 1;
		if (lt_lseq(str, CLSTR("throw"))) return 1;
		if (lt_lseq(str, CLSTR("true"))) return 1;
		if (lt_lseq(str, CLSTR("try"))) return 1;
		if (lt_lseq(str, CLSTR("typeof"))) return 1;
		break;

	case 'u':
		if (lt_lseq(str, CLSTR("unchecked"))) return 1;
		if (lt_lseq(str, CLSTR("unsafe"))) return 1;
		if (lt_lseq(str, CLSTR("using"))) return 1;
		break;

	case 'v':
		if (lt_lseq(str, CLSTR("virtual"))) return 1;
		if (lt_lseq(str, CLSTR("volatile"))) return 1;

		if (lt_lseq(str, CLSTR("var"))) return 1;
		break;

	case 'w':
		if (lt_lseq(str, CLSTR("while"))) return 1;

		if (lt_lseq(str, CLSTR("with"))) return 1;
		break;

	default:
		break;
	}
	return 0;
}

static
b8 is_datatype(lstr_t str) {
	if (!str.len)
		return 0;

	switch (str.str[0]) {
		case 'b':
			if (lt_lseq(str, CLSTR("bool"))) return 1;
			if (lt_lseq(str, CLSTR("byte"))) return 1;
			break;

		case 'c':
			if (lt_lseq(str, CLSTR("char"))) return 1;
			break;

		case 'd':
			if (lt_lseq(str, CLSTR("decimal"))) return 1;
			if (lt_lseq(str, CLSTR("double"))) return 1;
			break;

		case 'f':
			if (lt_lseq(str, CLSTR("float"))) return 1;
			break;

		case 'i':
			if (lt_lseq(str, CLSTR("int"))) return 1;
			break;

		case 'l':
			if (lt_lseq(str, CLSTR("long"))) return 1;
			break;

		case 'o':
			if (lt_lseq(str, CLSTR("object"))) return 1;
			break;

		case 's':
			if (lt_lseq(str, CLSTR("sbyte"))) return 1;
			if (lt_lseq(str, CLSTR("short"))) return 1;
			if (lt_lseq(str, CLSTR("string"))) return 1;
			break;

		case 'u':
			if (lt_lseq(str, CLSTR("uint"))) return 1;
			if (lt_lseq(str, CLSTR("ulong"))) return 1;
			if (lt_lseq(str, CLSTR("ushort"))) return 1;
			break;

		case 'v':
			if (lt_lseq(str, CLSTR("void"))) return 1;
			break;
	}
	return 0;
}

static
highl_t* gen_line(lstr_t line, multiline_mode_t* ml_mode, lt_arena_t* alloc) {
	highl_t* head = NULL;
	highl_t** node = &head;
	int c = 0;
	usz i = 0, start = 0;

	if (*ml_mode == MLMODE_COMMENT)
		goto parse_multiline_comment;

	while (i < line.len) {
		c = line.str[i];
		start = i++;

		hl_mode_t mode = HLM_UNKNOWN;

		switch (c) {
		case '/':
			if (i >= line.len) {
				mode = HLM_OPERATOR;
				break;
			}

			c = line.str[i];
			if (c == '/') {
				i = line.len;
				mode = HLM_COMMENT;
				break;
			}
			else if (c == '*') {
				*ml_mode = MLMODE_COMMENT;

			parse_multiline_comment:
				while (i < line.len) {
					c = line.str[i++];
					if (c == '*' && i < line.len && line.str[i] == '/') {
						++i;
						*ml_mode = MLMODE_NONE;
						break;
					}
				}
				mode = HLM_COMMENT;
				break;
			}

			mode = HLM_OPERATOR;
			break;

		case '#':
			mode = HLM_HASH;
			break;

		case '"':
			while (i < line.len) {
				c = line.str[i++];
				if (c == '"')
					break;
				if (c == '\\' && i < line.len)
					++i;
			}
			mode = HLM_STRING;
			break;

		case '\'':
			while (i < line.len) {
				c = line.str[i++];
				if (c == '\'')
					break;
				if (c == '\\' && i < line.len)
					++i;
			}
			mode = HLM_CHAR;
			break;

		case '_':
			mode = HLM_IDENTIFIER;
			break;

		default:
			if (lt_is_space(c)) {
				while (i < line.len) {
					c = line.str[i];
					if (!lt_is_space(c))
						break;
					++i;
				}
				mode = HLM_INDENT;
			}
			else if (lt_is_ident_head(c)) {
				mode = HLM_IDENTIFIER;

				while (i < line.len) {
					c = line.str[i];
					if (!lt_is_ident_body(c)) {
						if (c == '(')
							mode = HLM_FUNCTION;
						break;
					}
					++i;
				}

				lstr_t tk = LSTR(&line.str[start], i - start);

				if (is_keyword(tk))
					mode = HLM_KEYWORD;
				else if (is_datatype(tk))
					mode = HLM_DATATYPE;
			}
			else if (lt_is_numeric_head(c)) {
				while (i < line.len) {
					if (!lt_is_numeric_body(line.str[i]))
						break;
					++i;
				}
				mode = HLM_NUMBER;
			}
			else if (lt_is_operator(c))
				mode = HLM_OPERATOR;
			else if (lt_is_punc(c))
				mode = HLM_PUNCTUATION;
			break;
		}

		highl_t* new = lt_amalloc_lean(alloc, sizeof(highl_t));
		new->len = i - start;
		new->mode = mode;

		*node = new;
		node = &new->next;
	}
	*node = NULL;

	return head;
}

highl_t** hl_generate_cs(doc_t* doc, lt_arena_t* alloc) {
	usz line_count = lt_texted_line_count(&doc->ed);

	highl_t** lines = lt_amalloc_lean(alloc, line_count * sizeof(highl_t*));

	multiline_mode_t mode = MLMODE_NONE;
	for (usz i = 0; i < line_count; ++i)
		lines[i] = gen_line(lt_texted_line_str(&doc->ed, i), &mode, alloc);

	return lines;
}


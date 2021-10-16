// Copyright (C) 2021, Alex Edin <lutfisk@lutfisk.net>
// SPDX-License-Identifier: GPL-2.0+

#ifndef CLR_H
#define CLR_H

#define CLR_LINENUM_BG 1
#define CLR_LINENUM_FG 2

#define CLR_LINENUM_UFLOW_BG 3
#define CLR_LINENUM_UFLOW_FG 4

#define CLR_LINENUM_SEL_BG 5
#define CLR_LINENUM_SEL_FG 6

#define CLR_HEADER_TAB_BG 7
#define CLR_HEADER_TAB_FG 8

#define CLR_EDITOR_FG 9
#define CLR_EDITOR_BG 10

#define CLR_SYNTAX_UNKNOWN 11

#define CLR_SYNTAX_STRING 13
#define CLR_SYNTAX_CHAR 14
#define CLR_SYNTAX_NUMBER 15

#define CLR_SYNTAX_IDENTIFIER 16
#define CLR_SYNTAX_KEYWORD 17
#define CLR_SYNTAX_COMMENT 18
#define CLR_SYNTAX_DATATYPE 19

#define CLR_SYNTAX_HASH 20
#define CLR_SYNTAX_OPERATOR 22
#define CLR_SYNTAX_PUNCTUATION 23

#define CLR_SYNTAX_FUNCTION 24

#define CLR_NOTIFY_ERROR_FG 25
#define CLR_NOTIFY_ERROR_BG 26

#define CLR_BROWSE_FILES_INPUT_FG 27
#define CLR_BROWSE_FILES_INPUT_BG 28

#define CLR_BROWSE_FILES_ENTRY_FG 29
#define CLR_BROWSE_FILES_ENTRY_BG 30

#define CLR_BROWSE_FILES_SEL_FG 31
#define CLR_BROWSE_FILES_SEL_BG 32

#define CLR_HEADER_BG 33

#define CLR_SYNTAX_TRAIL_INDENT 34

#define PAIR_LINENUM 1
#define PAIR_LINENUM_UFLOW 2
#define PAIR_LINENUM_SEL 3
#define PAIR_HEADER_TAB 4
#define PAIR_EDITOR 5

#define PAIR_SYNTAX_UNKNOWN 11

#define PAIR_SYNTAX_STRING 13
#define PAIR_SYNTAX_CHAR 14
#define PAIR_SYNTAX_NUMBER 15

#define PAIR_SYNTAX_IDENTIFIER 16
#define PAIR_SYNTAX_KEYWORD 17
#define PAIR_SYNTAX_COMMENT 18
#define PAIR_SYNTAX_DATATYPE 19

#define PAIR_SYNTAX_HASH 20
#define PAIR_SYNTAX_BRACKET 21
#define PAIR_SYNTAX_OPERATOR 22
#define PAIR_SYNTAX_PUNCTUATION 23
#define PAIR_SYNTAX_FUNCTION 24

#define PAIR_NOTIFY_ERROR 25

#define PAIR_BROWSE_FILES_INPUT 26
#define PAIR_BROWSE_FILES_ENTRY 27
#define PAIR_BROWSE_FILES_SEL 28

#define PAIR_HEADER_BG 29

#define PAIR_SYNTAX_TRAIL_INDENT 30

typedef struct conf conf_t;

void clr_push();
void clr_pop();

void clr_load(conf_t* clr_conf);

#endif

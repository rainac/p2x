/*
This file is part of P2X.
Copyright © 2018 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#include "token.ncd.enum.hh"
#include <string.h>

/*!max:re2c*/

typedef unsigned char uint8_t;

int re2c_scanner_c_lex(const uint8_t *&s, const uint8_t *&m, uint8_t const * lim)
{

  /*!stags:re2c format = 'const char *@@;'; */

  lim += YYMAXFILL;

    /*!re2c
	re2c:define:YYCTYPE = uint8_t;
	re2c:define:YYCURSOR = s;
	re2c:define:YYMARKER = m;
	re2c:define:YYLIMIT = lim;
	re2c:define:YYFILL = "return false;";
	re2c:define:YYFILL:naked = 1;

	UNDERSCORE = "_";

	DIGIT = [0-9];
	LOWER = [a-z];
	UPPER =  [A-Z];

	HIGHLET = [\xC0-\xDF][\x80-\xBF]|[\xE0-\xEF][\x80-\xBF][\x80-\xBF]|[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF];
	LETTER  = LOWER|UPPER|HIGHLET;
	IDINIT  = LETTER|[_];
	ID      = IDINIT(LETTER|DIGIT|UNDERSCORE)*;
        INT     = ("0x")?DIGIT+[uUlL]*;
	FLOAT1  = DIGIT+"."DIGIT+;
	FLOAT2  = DIGIT+("."DIGIT*)?;
	FLOAT3  = DIGIT*"."DIGIT+;
	FLOATSUFFIX = ([eEdD][+-]?DIGIT+)?[lLfF]?;
	FLOAT   = (FLOAT1|FLOAT2|FLOAT3)FLOATSUFFIX;
	STRING1_ESC_CONTENT = "\\\"";
	STRING2_ESC_CONTENT = "\\'";
	STRING1 = "\"" ([^\"\n\x00] | STRING1_ESC_CONTENT)* "\"";
	STRING2 = "'" ([^\'\n\x00] | STRING2_ESC_CONTENT)* "'";
	STRING  = STRING1|STRING2;

        ID     { return TOKEN_IDENTIFIER; }
        INT    { return TOKEN_INTEGER; }
        FLOAT  { return TOKEN_FLOAT; }
        STRING { return TOKEN_STRING; }

	" "+  { return TOKEN_SPACE; }
	"\n" { return TOKEN_NEWLINE; }
	"\t" { return TOKEN_TAB; }
	"\r" { return TOKEN_CRETURN; }

	"(" { return TOKEN_L_PAREN; }
	")" { return TOKEN_R_PAREN; }

	"{" { return TOKEN_L_BRACE; }
	"}" { return TOKEN_R_BRACE; }

	"[" { return TOKEN_L_BRACKET; }
	"]" { return TOKEN_R_BRACKET; }

	"="              { return TOKEN_EQUAL; }

	"->"             { return TOKEN_MINUS_GREATER; }

	"<<"             { return TOKEN_DOUBLE_LESS; }
	">>"             { return TOKEN_DOUBLE_GREATER; }

	"<"              { return TOKEN_LESS; }
	">"              { return TOKEN_GREATER; }

	"<="             { return TOKEN_LE; }
	">="             { return TOKEN_GE; }

	"=="             { return TOKEN_DOUBLE_EQUAL; }
	"!="             { return TOKEN_NOT_EQUAL; }
	"~="             { return TOKEN_NOT_EQUAL; }

	"~"              { return TOKEN_TILDE; }
	"!"              { return TOKEN_EXCLAM; }
	"?"              { return TOKEN_QUESTION; }
	":"              { return TOKEN_COLON; }
	"::"             { return TOKEN_DOUBLE_COLON; }
	","              { return TOKEN_COMMA; }
	";"              { return TOKEN_SEMICOLON; }
	"."              { return TOKEN_FULL_STOP; }

	"&"              { return TOKEN_AND; }
	"&&"             { return TOKEN_DOUBLE_AND; }
	"|"              { return TOKEN_OR; }
	"||"             { return TOKEN_DOUBLE_OR; }
	"^"              { return TOKEN_POW; }

	"++"             { return TOKEN_DOUBLE_PLUS; }
	"--"             { return TOKEN_DOUBLE_MINUS; }

	"+"              { return TOKEN_PLUS; }
	"-"              { return TOKEN_MINUS; }
	"/"              { return TOKEN_DIV; }
	"*"              { return TOKEN_MULT; }
	"%"              { return TOKEN_MOD; }

	"&="             { return TOKEN_AND_EQUAL; }
	"|="             { return TOKEN_OR_EQUAL; }
	"^="             { return TOKEN_POW_EQUAL; }

	"+="             { return TOKEN_PLUS_EQUAL; }
	"-="             { return TOKEN_MINUS_EQUAL; }
	"/="             { return TOKEN_DIV_EQUAL; }
	"*="             { return TOKEN_MULT_EQUAL; }
	"%="             { return TOKEN_MOD_EQUAL; }

	"<<="            { return TOKEN_DOUBLE_LESS_EQUAL; }
	">>="            { return TOKEN_DOUBLE_GREATER_EQUAL; }

	"#"              { return TOKEN_HASH; }

	"//"             { return TOKEN_DOUBLE_DIV; }
	"/*"             { return TOKEN_C_COMMENT_START; }
	"*/"             { return TOKEN_C_COMMENT_END; }

        "$"              { return TOKEN_DOLLAR; }
        "\\"             { return TOKEN_BACKSLASH; }

	"\x00"           { return TOKEN_EOF; }

	*   { return TOKEN_ILLEGAL_CHAR; }

	*/
}

/* Local Variables: */
/* mode: c */
/* coding: latin-1 */
/* End: */

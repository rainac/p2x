/*
This file is part of P2X.
Copyright � 2018 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#include "token.ncd.enum.hh"
#include <string.h>

/*!max:re2c*/

typedef unsigned char uint8_t;

int re2c_scanner_r_lex(const uint8_t *&s, const uint8_t *&m, uint8_t const * lim)
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

SPACE = " ";
NL    = "\n";
CR    = "\r";
TAB   = "\t";

UNDERSCORE = "_";

DOT = ".";
WHITESEQ = (CR|NL|TAB|SPACE)+;
DOTSPACE = WHITESEQ DOT;

DIGIT = [0-9];
LOWER = [a-z];
UPPER = [A-Z];
/*
thanks to the idea from AProgrammer at
http://stackoverflow.com/questions/8447701/c-parsing-library-with-utf-8-support
*/
HIGHLET = [\xC0-\xDF][\x80-\xBF]|[\xE0-\xEF][\x80-\xBF][\x80-\xBF]|[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF];
LETTER = LOWER|UPPER|HIGHLET;
IDINIT = LETTER|DOT;
ID    = IDINIT(IDINIT|DIGIT|UNDERSCORE)*;
WHITE = SPACE|[\t\r\v];
INT   = ("0x")?DIGIT+[uUlL]*;
FLOAT1 = DIGIT+"."DIGIT+;
FLOAT2 = DIGIT+("."DIGIT*)?;
FLOAT3 = DIGIT*"."DIGIT+;
FLOATSUFFIX = ([eEdD][+-]?DIGIT+)?[i]?;
FLOAT = (FLOAT1|FLOAT2|FLOAT3)FLOATSUFFIX;
STRING1_ESC_CONTENT = "\\\"";
STRING2_ESC_CONTENT = "\\'";
STRING1 = "\"" ([^\"\n\x00] | STRING1_ESC_CONTENT)* "\"";
STRING2 = "'" ([^\'\n\x00] | STRING2_ESC_CONTENT)* "'";
STRING = STRING1|STRING2;

INT                     { return TOKEN_INTEGER; }
STRING                  { return TOKEN_STRING; }
FLOAT                   { return TOKEN_FLOAT; }

"("                     { return TOKEN_L_PAREN; }
")"                     { return TOKEN_R_PAREN; }

"{"                     { return TOKEN_L_BRACE; }
"}"                     { return TOKEN_R_BRACE; }

"["                     { return TOKEN_L_BRACKET; }
"]"                     { return TOKEN_R_BRACKET; }

"[["                    { return TOKEN_DOUBLE_L_BRACKET; }
"]]"                    { return TOKEN_DOUBLE_R_BRACKET; }

"="                     { return TOKEN_EQUAL; }

"<-"                    { return TOKEN_LEFT_ARROW; }
"<<-"                   { return TOKEN_DOUBLE_LEFT_ARROW; }
"->"                    { return TOKEN_RIGHT_ARROW; }
"->>"                   { return TOKEN_DOUBLE_RIGHT_ARROW; }

"<"                     { return TOKEN_LESS; }
">"                     { return TOKEN_GREATER; }

"<="                    { return TOKEN_LE; }
">="                    { return TOKEN_GE; }

"=="                    { return TOKEN_DOUBLE_EQUAL; }
"!="                    { return TOKEN_NOT_EQUAL; }

"~"                     { return TOKEN_TILDE; }
"!"                     { return TOKEN_EXCLAM; }
"?"                     { return TOKEN_QUESTION; }
":"                     { return TOKEN_COLON; }
"::"                    { return TOKEN_DOUBLE_COLON; }
":::"                   { return TOKEN_TRIPLE_COLON; }
","                     { return TOKEN_COMMA; }
";"                     { return TOKEN_SEMICOLON; }
"."                     { return TOKEN_FULL_STOP; }
"@"                     { return TOKEN_AT; }
"&"                     { return TOKEN_AND; }
"&&"                    { return TOKEN_DOUBLE_AND; }
"|"                     { return TOKEN_OR; }
"||"                    { return TOKEN_DOUBLE_OR; }

"+"                     { return TOKEN_PLUS; }
"-"                     { return TOKEN_MINUS; }
"/"                     { return TOKEN_DIV; }
"\\"                    { return TOKEN_BACKSLASH; }
"*"                     { return TOKEN_MULT; }
"%"                     { return TOKEN_MOD; }
"%"[^ %]*"%"            { return TOKEN_OPERATOR; }
"^"                     { return TOKEN_POW; }
"_"                     { return TOKEN_UNDERSCORE; }
"$"                     { return TOKEN_DOLLAR; }
"�"                     { return TOKEN_CIRC; }
"#"                     { return TOKEN_HASH; }
"\""                    { return TOKEN_QUOTE; }
"'"                     { return TOKEN_APOS; }

CR                      { return TOKEN_CRETURN; }
NL                      { return TOKEN_NEWLINE; }
TAB                     { return TOKEN_TAB; }
SPACE+                  { return TOKEN_SPACE; }
ID                      { return TOKEN_IDENTIFIER; }


	"\x00"           { return TOKEN_EOF; }

	*   { return TOKEN_ILLEGAL_CHAR; }

	*/
}

/* Local Variables: */
/* mode: c */
/* coding: latin-1 */
/* End: */

/*
This file is part of P2X.
Copyright © 2018 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#include "token.ncd.enum.hh"

/*!max:re2c*/

/*!types:re2c*/

typedef unsigned char uint8_t;

static int re2c_scanner_m_c = yycINITIAL;

int re2c_scanner_m_lex(const uint8_t *&s, const uint8_t *&m, uint8_t const * lim)
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
        re2c:define:YYGETCONDITION = "re2c_scanner_m_c";
        re2c:define:YYGETCONDITION:naked = 1;
        re2c:define:YYSETCONDITION = "re2c_scanner_m_c = @@;";
        re2c:define:YYSETCONDITION:naked = 1;

SPACE =  " ";
NL =  "\n";
CR =  "\r";
TAB = "\t";
UNDERSCORE = "_";

DIGIT = [0-9];
LOWER = [a-z];
UPPER = [A-Z];
/*
thanks to the idea from AProgrammer at
http://stackoverflow.com/questions/8447701/c-parsing-library-with-utf-8-support
*/
HIGHLET = [\xC0-\xDF][\x80-\xBF]|[\xE0-\xEF][\x80-\xBF][\x80-\xBF]|[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF];
LETTER = LOWER|UPPER|HIGHLET;
IDINIT = LETTER|[_];
ID     = IDINIT(LETTER|DIGIT|UNDERSCORE)*;
WHITE  = SPACE|[\t\r\v];
INT    = ("0x")?DIGIT+[uUlL]*;
FLOAT1 = DIGIT+"."DIGIT+;
FLOAT2 = DIGIT+("."DIGIT+)?;
FLOAT3 = DIGIT*"."DIGIT+;
FLOATSUFFIX = ([eEdD][+-]?DIGIT+)?[lLfFiI]?;
FLOAT  = (FLOAT1|FLOAT2|FLOAT3)FLOATSUFFIX;
STRING1_ESC_CONTENT = "\\\"";
STRING1 = "\"" ([^\"\n\x00] | STRING1_ESC_CONTENT)* "\"";
STRING2 = "'" [^\'\n\x00]* "'";

<INITIAL, STARTEXP> INT  => INITIAL  { return TOKEN_INTEGER; }
<*> STRING1              => STARTEXP { return TOKEN_STRING; }
<STARTEXP> STRING2       => STARTEXP { return TOKEN_STRING; }
<*> FLOAT                => INITIAL  { return TOKEN_FLOAT; }
<*> DIGIT+"."/[^.*/\\^]  => INITIAL  { return TOKEN_FLOAT; }

<*> "("                  => STARTEXP { return TOKEN_L_PAREN; }
<*> ")"                  => INITIAL { return TOKEN_R_PAREN; }

<*> "{"                  => STARTEXP { return TOKEN_L_BRACE; }
<*> "}"                  => INITIAL { return TOKEN_R_BRACE; }

<*> "["                  => STARTEXP { return TOKEN_L_BRACKET; }
<*> "]"                  => INITIAL { return TOKEN_R_BRACKET; }

<*> "="                  => STARTEXP { return TOKEN_EQUAL; }

<*> "<"                  => STARTEXP { return TOKEN_LESS; }
<*> ">"                  => STARTEXP { return TOKEN_GREATER; }

<*> "<="                 => STARTEXP { return TOKEN_LE; }
<*> ">="                 => STARTEXP { return TOKEN_GE; }

<*> "=="                 => STARTEXP { return TOKEN_DOUBLE_EQUAL; }
<*> "!="                 => STARTEXP { return TOKEN_NOT_EQUAL; }
<*> "~="                 => STARTEXP { return TOKEN_NOT_EQUAL; }

<*> "~"                  => STARTEXP { return TOKEN_TILDE; }
<*> "!"                  => STARTEXP { return TOKEN_EXCLAM; }
<*> "?"                  => STARTEXP { return TOKEN_QUESTION; }
<*> ":"                  => STARTEXP { return TOKEN_COLON; }
<*> ","                  => STARTEXP { return TOKEN_COMMA; }
<*> ";"                  => STARTEXP { return TOKEN_SEMICOLON; }
<*> "."                  => STARTEXP { return TOKEN_FULL_STOP; }

<*> "&"                  => STARTEXP { return TOKEN_AND; }
<*> "&&"                 => STARTEXP { return TOKEN_DOUBLE_AND; }
<*> "|"                  => STARTEXP { return TOKEN_OR; }
<*> "||"                 => STARTEXP { return TOKEN_DOUBLE_OR; }

<*> "+"                  => STARTEXP { return TOKEN_PLUS; }
<*> "-"                  => STARTEXP { return TOKEN_MINUS; }
<*> "/"                  => STARTEXP { return TOKEN_DIV; }
<*> "*"                  => STARTEXP { return TOKEN_MULT; }
<*> "%"                  => STARTEXP { return TOKEN_MOD; }
<*> "\\"                 => STARTEXP { return TOKEN_BACKSLASH; }
<*> "^"                  => STARTEXP { return TOKEN_POW; }
<*> "'"                  => STARTEXP { return TOKEN_APOS; }

<*> ".*"                 => STARTEXP { return TOKEN_DOTMULT; }
<*> "./"                 => STARTEXP { return TOKEN_DOTDIV; }
<*> ".\\"                => STARTEXP { return TOKEN_DOTBACKSLASH; }
<*> ".^"                 => STARTEXP { return TOKEN_DOTPOW; }
<*> ".'"                 => STARTEXP { return TOKEN_DOTAPOS; }

<*> "+="                 => STARTEXP { return TOKEN_PLUS_EQUAL; }
<*> "-="                 => STARTEXP { return TOKEN_MINUS_EQUAL; }
<*> "/="                 => STARTEXP { return TOKEN_DIV_EQUAL; }
<*> "\\="                => STARTEXP { return TOKEN_BACKSLASH_EQUAL; }
<*> "*="                 => STARTEXP { return TOKEN_MULT_EQUAL; }
<*> "^="                 => STARTEXP { return TOKEN_POW_EQUAL; }

<*> "./="                => STARTEXP { return TOKEN_DOT_DIV_EQUAL; }
<*> ".\\="               => STARTEXP { return TOKEN_DOT_BACKSLASH_EQUAL; }
<*> ".*="                => STARTEXP { return TOKEN_DOT_MULT_EQUAL; }
<*> ".^="                => STARTEXP { return TOKEN_DOT_POW_EQUAL; }

<*> "@"                  => STARTEXP { return TOKEN_AT; }

<*> "#"                  => INITIAL { return TOKEN_HASH; }

<*> CR                   => STARTEXP { return TOKEN_CRETURN; }
<*> NL                   => STARTEXP { return TOKEN_NEWLINE; }
<*> TAB                  => STARTEXP { return TOKEN_TAB; }
<*> SPACE+               => STARTEXP { return TOKEN_SPACE; }
<*> ID                   => INITIAL { return TOKEN_IDENTIFIER; }

<*> "..""."+             => STARTEXP { return TOKEN_TRIPLE_DOT; }

<*> "\x00"               => INITIAL { return TOKEN_EOF; }

<*> *                    => INITIAL { return TOKEN_ILLEGAL_CHAR; }

*/
}

/* Local Variables: */
/* mode: c */
/* coding: latin-1 */
/* End: */

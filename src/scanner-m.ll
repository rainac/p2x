/*
This file is part of P2X.
Copyright © 2018 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

%{

#include "token.ncd.enum.hh"

#define yystype ParserToken

%}

%option c++
%s STARTEXP

SPACE " "
NL    "\n"
CR    "\r"
TAB   "\t"
UNDERSCORE   "_"

DIGIT [0-9]
LOWER [a-z]
UPPER [A-Z]
/*
thanks to the idea from AProgrammer at
http://stackoverflow.com/questions/8447701/c-parsing-library-with-utf-8-support
*/
HIGHLET [\xC0-\xDF][\x80-\xBF]|[\xE0-\xEF][\x80-\xBF][\x80-\xBF]|[\xF0-\xF7][\x80-\xBF][\x80-\xBF][\x80-\xBF]
LETTER {LOWER}|{UPPER}|{HIGHLET}
IDINIT {LETTER}|[_]
ID    {IDINIT}({LETTER}|{DIGIT}|{UNDERSCORE})*
WHITE {SPACE}|[\t\r\v]
INT   ("0x")?{DIGIT}+[uUlL]*
FLOAT1 {DIGIT}+"."{DIGIT}+
FLOAT2 {DIGIT}+("."{DIGIT}+)?
FLOAT3 {DIGIT}*"."{DIGIT}+
FLOATSUFFIX ([eEdD][+-]?{DIGIT}+)?[lLfFiI]?
FLOAT ({FLOAT1}|{FLOAT2}|{FLOAT3}){FLOATSUFFIX}
STRING1 "\""[^\"\n]*"\""
STRING2 "'"[^\'\n]*"'"

%%

{INT}                   BEGIN(INITIAL);  return TOKEN_INTEGER;
{STRING1}               BEGIN(STARTEXP); return TOKEN_STRING;
<STARTEXP>{STRING2}     BEGIN(STARTEXP); return TOKEN_STRING;
{FLOAT}                 BEGIN(INITIAL);  return TOKEN_FLOAT;
{DIGIT}+"."/[^.*/\\^]   BEGIN(INITIAL);  return TOKEN_FLOAT;

"("                     BEGIN(STARTEXP); return TOKEN_L_PAREN;
")"                     BEGIN(INITIAL); return TOKEN_R_PAREN;

"{"                     BEGIN(STARTEXP); return TOKEN_L_BRACE;
"}"                     BEGIN(INITIAL); return TOKEN_R_BRACE;

"["                     BEGIN(STARTEXP); return TOKEN_L_BRACKET;
"]"                     BEGIN(INITIAL); return TOKEN_R_BRACKET;

"="                     BEGIN(STARTEXP); return TOKEN_EQUAL;

"<"                     BEGIN(STARTEXP); return TOKEN_LESS;
">"                     BEGIN(STARTEXP); return TOKEN_GREATER;

"<="                    BEGIN(STARTEXP); return TOKEN_LE;
">="                    BEGIN(STARTEXP); return TOKEN_GE;

"=="                    BEGIN(STARTEXP); return TOKEN_DOUBLE_EQUAL;
"!="                    BEGIN(STARTEXP); return TOKEN_NOT_EQUAL;
"~="                    BEGIN(STARTEXP); return TOKEN_NOT_EQUAL;

"~"                     BEGIN(STARTEXP); return TOKEN_TILDE;
"!"                     BEGIN(STARTEXP); return TOKEN_EXCLAM;
"?"                     BEGIN(STARTEXP); return TOKEN_QUESTION;
":"                     BEGIN(STARTEXP); return TOKEN_COLON;
","                     BEGIN(STARTEXP); return TOKEN_COMMA;
";"                     BEGIN(STARTEXP); return TOKEN_SEMICOLON;
"."                     BEGIN(STARTEXP); return TOKEN_FULL_STOP;

"&"                     BEGIN(STARTEXP); return TOKEN_AND;
"&&"                    BEGIN(STARTEXP); return TOKEN_DOUBLE_AND;
"|"                     BEGIN(STARTEXP); return TOKEN_OR;
"||"                    BEGIN(STARTEXP); return TOKEN_DOUBLE_OR;

"+"                     BEGIN(STARTEXP); return TOKEN_PLUS;
"-"                     BEGIN(STARTEXP); return TOKEN_MINUS;
"/"                     BEGIN(STARTEXP); return TOKEN_DIV;
"*"                     BEGIN(STARTEXP); return TOKEN_MULT;
"%"                     BEGIN(STARTEXP); return TOKEN_MOD;

"*"                     BEGIN(STARTEXP); return TOKEN_MULT;
"/"                     BEGIN(STARTEXP); return TOKEN_DIV;
"\\"                    BEGIN(STARTEXP); return TOKEN_BACKSLASH;
"^"                     BEGIN(STARTEXP); return TOKEN_POW;
"'"                     BEGIN(STARTEXP); return TOKEN_APOS;

".*"                    BEGIN(STARTEXP); return TOKEN_DOTMULT;
"./"                    BEGIN(STARTEXP); return TOKEN_DOTDIV;
".\\"                   BEGIN(STARTEXP); return TOKEN_DOTBACKSLASH;
".^"                    BEGIN(STARTEXP); return TOKEN_DOTPOW;
".'"                    BEGIN(STARTEXP); return TOKEN_DOTAPOS;

"+="                    BEGIN(STARTEXP); return TOKEN_PLUS_EQUAL;
"-="                    BEGIN(STARTEXP); return TOKEN_MINUS_EQUAL;
"/="                    BEGIN(STARTEXP); return TOKEN_DIV_EQUAL;
"\\="                   BEGIN(STARTEXP); return TOKEN_BACKSLASH_EQUAL;
"*="                    BEGIN(STARTEXP); return TOKEN_MULT_EQUAL;
"^="                    BEGIN(STARTEXP); return TOKEN_POW_EQUAL;

"./="                   BEGIN(STARTEXP); return TOKEN_DOT_DIV_EQUAL;
".\\="                  BEGIN(STARTEXP); return TOKEN_DOT_BACKSLASH_EQUAL;
".*="                   BEGIN(STARTEXP); return TOKEN_DOT_MULT_EQUAL;
".^="                   BEGIN(STARTEXP); return TOKEN_DOT_POW_EQUAL;

"#"                     BEGIN(INITIAL); return TOKEN_HASH;

{CR}                    BEGIN(STARTEXP); return TOKEN_CRETURN;
{NL}                    BEGIN(STARTEXP); return TOKEN_NEWLINE;
{TAB}                   BEGIN(STARTEXP); return TOKEN_TAB;
{SPACE}+                BEGIN(STARTEXP); return TOKEN_SPACE;
{ID}                    BEGIN(INITIAL); return TOKEN_IDENTIFIER;

"..""."+                BEGIN(STARTEXP); return TOKEN_TRIPLE_DOT;

.                       BEGIN(INITIAL); return TOKEN_ILLEGAL_CHAR;

%%

int yyFlexLexer::yywrap() { return 1; }

/*
$Id$
*/

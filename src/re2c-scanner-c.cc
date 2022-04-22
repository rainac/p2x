/* Generated by re2c 3.0 on Tue Mar 29 20:46:52 2022 */
#line 1 "re2c-scanner-c.re"
/*
This file is part of P2X.
Copyright � 2018 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#include "token.ncd.enum.hh"
#include <string.h>

#line 13 "re2c-scanner-c.cc"
#define YYMAXFILL 4
#line 10 "re2c-scanner-c.re"


typedef unsigned char uint8_t;

int re2c_scanner_c_lex(const uint8_t *&s, const uint8_t *&m, uint8_t const * lim)
{

  
#line 24 "re2c-scanner-c.cc"
#line 17 "re2c-scanner-c.re"


  lim += YYMAXFILL;

    
#line 31 "re2c-scanner-c.cc"
{
	uint8_t yych;
	unsigned int yyaccept = 0;
	if ((lim - s) < 4) return false;
	yych = *s;
	switch (yych) {
		case 0x00: goto yy1;
		case '\t': goto yy4;
		case '\n': goto yy5;
		case '\r': goto yy6;
		case ' ': goto yy7;
		case '!': goto yy9;
		case '"': goto yy11;
		case '#': goto yy12;
		case '$': goto yy13;
		case '%': goto yy14;
		case '&': goto yy16;
		case '\'': goto yy18;
		case '(': goto yy19;
		case ')': goto yy20;
		case '*': goto yy21;
		case '+': goto yy23;
		case ',': goto yy25;
		case '-': goto yy26;
		case '.': goto yy28;
		case '/': goto yy30;
		case '0': goto yy32;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy34;
		case ':': goto yy35;
		case ';': goto yy37;
		case '<': goto yy38;
		case '=': goto yy40;
		case '>': goto yy42;
		case '?': goto yy44;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '_':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z': goto yy45;
		case '[': goto yy47;
		case '\\': goto yy48;
		case ']': goto yy49;
		case '^': goto yy50;
		case '{': goto yy52;
		case '|': goto yy53;
		case '}': goto yy55;
		case '~': goto yy56;
		case 0xC0:
		case 0xC1:
		case 0xC2:
		case 0xC3:
		case 0xC4:
		case 0xC5:
		case 0xC6:
		case 0xC7:
		case 0xC8:
		case 0xC9:
		case 0xCA:
		case 0xCB:
		case 0xCC:
		case 0xCD:
		case 0xCE:
		case 0xCF:
		case 0xD0:
		case 0xD1:
		case 0xD2:
		case 0xD3:
		case 0xD4:
		case 0xD5:
		case 0xD6:
		case 0xD7:
		case 0xD8:
		case 0xD9:
		case 0xDA:
		case 0xDB:
		case 0xDC:
		case 0xDD:
		case 0xDE:
		case 0xDF: goto yy58;
		case 0xE0:
		case 0xE1:
		case 0xE2:
		case 0xE3:
		case 0xE4:
		case 0xE5:
		case 0xE6:
		case 0xE7:
		case 0xE8:
		case 0xE9:
		case 0xEA:
		case 0xEB:
		case 0xEC:
		case 0xED:
		case 0xEE:
		case 0xEF: goto yy59;
		case 0xF0:
		case 0xF1:
		case 0xF2:
		case 0xF3:
		case 0xF4:
		case 0xF5:
		case 0xF6:
		case 0xF7: goto yy60;
		default: goto yy2;
	}
yy1:
	++s;
#line 133 "re2c-scanner-c.re"
	{ return TOKEN_EOF; }
#line 197 "re2c-scanner-c.cc"
yy2:
	++s;
yy3:
#line 135 "re2c-scanner-c.re"
	{ return TOKEN_ILLEGAL_CHAR; }
#line 203 "re2c-scanner-c.cc"
yy4:
	++s;
#line 58 "re2c-scanner-c.re"
	{ return TOKEN_TAB; }
#line 208 "re2c-scanner-c.cc"
yy5:
	++s;
#line 57 "re2c-scanner-c.re"
	{ return TOKEN_NEWLINE; }
#line 213 "re2c-scanner-c.cc"
yy6:
	++s;
#line 59 "re2c-scanner-c.re"
	{ return TOKEN_CRETURN; }
#line 218 "re2c-scanner-c.cc"
yy7:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case ' ': goto yy7;
		default: goto yy8;
	}
yy8:
#line 56 "re2c-scanner-c.re"
	{ return TOKEN_SPACE; }
#line 230 "re2c-scanner-c.cc"
yy9:
	yych = *++s;
	switch (yych) {
		case '=': goto yy61;
		default: goto yy10;
	}
yy10:
#line 88 "re2c-scanner-c.re"
	{ return TOKEN_EXCLAM; }
#line 240 "re2c-scanner-c.cc"
yy11:
	yyaccept = 0;
	yych = *(m = ++s);
	switch (yych) {
		case 0x00:
		case '\n': goto yy3;
		default: goto yy63;
	}
yy12:
	++s;
#line 124 "re2c-scanner-c.re"
	{ return TOKEN_HASH; }
#line 253 "re2c-scanner-c.cc"
yy13:
	++s;
#line 130 "re2c-scanner-c.re"
	{ return TOKEN_DOLLAR; }
#line 258 "re2c-scanner-c.cc"
yy14:
	yych = *++s;
	switch (yych) {
		case '=': goto yy68;
		default: goto yy15;
	}
yy15:
#line 109 "re2c-scanner-c.re"
	{ return TOKEN_MOD; }
#line 268 "re2c-scanner-c.cc"
yy16:
	yych = *++s;
	switch (yych) {
		case '&': goto yy69;
		case '=': goto yy70;
		default: goto yy17;
	}
yy17:
#line 96 "re2c-scanner-c.re"
	{ return TOKEN_AND; }
#line 279 "re2c-scanner-c.cc"
yy18:
	yyaccept = 0;
	yych = *(m = ++s);
	switch (yych) {
		case 0x00:
		case '\n': goto yy3;
		default: goto yy72;
	}
yy19:
	++s;
#line 61 "re2c-scanner-c.re"
	{ return TOKEN_L_PAREN; }
#line 292 "re2c-scanner-c.cc"
yy20:
	++s;
#line 62 "re2c-scanner-c.re"
	{ return TOKEN_R_PAREN; }
#line 297 "re2c-scanner-c.cc"
yy21:
	yych = *++s;
	switch (yych) {
		case '/': goto yy74;
		case '=': goto yy75;
		default: goto yy22;
	}
yy22:
#line 108 "re2c-scanner-c.re"
	{ return TOKEN_MULT; }
#line 308 "re2c-scanner-c.cc"
yy23:
	yych = *++s;
	switch (yych) {
		case '+': goto yy76;
		case '=': goto yy77;
		default: goto yy24;
	}
yy24:
#line 105 "re2c-scanner-c.re"
	{ return TOKEN_PLUS; }
#line 319 "re2c-scanner-c.cc"
yy25:
	++s;
#line 92 "re2c-scanner-c.re"
	{ return TOKEN_COMMA; }
#line 324 "re2c-scanner-c.cc"
yy26:
	yych = *++s;
	switch (yych) {
		case '-': goto yy78;
		case '=': goto yy79;
		case '>': goto yy80;
		default: goto yy27;
	}
yy27:
#line 106 "re2c-scanner-c.re"
	{ return TOKEN_MINUS; }
#line 336 "re2c-scanner-c.cc"
yy28:
	yych = *++s;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy81;
		default: goto yy29;
	}
yy29:
#line 94 "re2c-scanner-c.re"
	{ return TOKEN_FULL_STOP; }
#line 355 "re2c-scanner-c.cc"
yy30:
	yych = *++s;
	switch (yych) {
		case '*': goto yy84;
		case '/': goto yy85;
		case '=': goto yy86;
		default: goto yy31;
	}
yy31:
#line 107 "re2c-scanner-c.re"
	{ return TOKEN_DIV; }
#line 367 "re2c-scanner-c.cc"
yy32:
	yyaccept = 1;
	yych = *(m = ++s);
	switch (yych) {
		case '.': goto yy81;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy34;
		case 'D':
		case 'E':
		case 'F':
		case 'd':
		case 'e':
		case 'f': goto yy82;
		case 'L':
		case 'U':
		case 'l':
		case 'u': goto yy89;
		case 'x': goto yy90;
		default: goto yy33;
	}
yy33:
#line 52 "re2c-scanner-c.re"
	{ return TOKEN_INTEGER; }
#line 399 "re2c-scanner-c.cc"
yy34:
	yyaccept = 1;
	m = ++s;
	if ((lim - s) < 3) return false;
	yych = *s;
	switch (yych) {
		case '.': goto yy81;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy34;
		case 'D':
		case 'E':
		case 'd':
		case 'e': goto yy87;
		case 'F':
		case 'f': goto yy88;
		case 'L':
		case 'U':
		case 'l':
		case 'u': goto yy89;
		default: goto yy33;
	}
yy35:
	yych = *++s;
	switch (yych) {
		case ':': goto yy91;
		default: goto yy36;
	}
yy36:
#line 90 "re2c-scanner-c.re"
	{ return TOKEN_COLON; }
#line 438 "re2c-scanner-c.cc"
yy37:
	++s;
#line 93 "re2c-scanner-c.re"
	{ return TOKEN_SEMICOLON; }
#line 443 "re2c-scanner-c.cc"
yy38:
	yych = *++s;
	switch (yych) {
		case '<': goto yy92;
		case '=': goto yy94;
		default: goto yy39;
	}
yy39:
#line 77 "re2c-scanner-c.re"
	{ return TOKEN_LESS; }
#line 454 "re2c-scanner-c.cc"
yy40:
	yych = *++s;
	switch (yych) {
		case '=': goto yy95;
		default: goto yy41;
	}
yy41:
#line 70 "re2c-scanner-c.re"
	{ return TOKEN_EQUAL; }
#line 464 "re2c-scanner-c.cc"
yy42:
	yych = *++s;
	switch (yych) {
		case '=': goto yy96;
		case '>': goto yy97;
		default: goto yy43;
	}
yy43:
#line 78 "re2c-scanner-c.re"
	{ return TOKEN_GREATER; }
#line 475 "re2c-scanner-c.cc"
yy44:
	++s;
#line 89 "re2c-scanner-c.re"
	{ return TOKEN_QUESTION; }
#line 480 "re2c-scanner-c.cc"
yy45:
	yyaccept = 2;
	m = ++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '_':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z': goto yy45;
		case 0xC0:
		case 0xC1:
		case 0xC2:
		case 0xC3:
		case 0xC4:
		case 0xC5:
		case 0xC6:
		case 0xC7:
		case 0xC8:
		case 0xC9:
		case 0xCA:
		case 0xCB:
		case 0xCC:
		case 0xCD:
		case 0xCE:
		case 0xCF:
		case 0xD0:
		case 0xD1:
		case 0xD2:
		case 0xD3:
		case 0xD4:
		case 0xD5:
		case 0xD6:
		case 0xD7:
		case 0xD8:
		case 0xD9:
		case 0xDA:
		case 0xDB:
		case 0xDC:
		case 0xDD:
		case 0xDE:
		case 0xDF: goto yy99;
		case 0xE0:
		case 0xE1:
		case 0xE2:
		case 0xE3:
		case 0xE4:
		case 0xE5:
		case 0xE6:
		case 0xE7:
		case 0xE8:
		case 0xE9:
		case 0xEA:
		case 0xEB:
		case 0xEC:
		case 0xED:
		case 0xEE:
		case 0xEF: goto yy100;
		case 0xF0:
		case 0xF1:
		case 0xF2:
		case 0xF3:
		case 0xF4:
		case 0xF5:
		case 0xF6:
		case 0xF7: goto yy101;
		default: goto yy46;
	}
yy46:
#line 51 "re2c-scanner-c.re"
	{ return TOKEN_IDENTIFIER; }
#line 611 "re2c-scanner-c.cc"
yy47:
	++s;
#line 67 "re2c-scanner-c.re"
	{ return TOKEN_L_BRACKET; }
#line 616 "re2c-scanner-c.cc"
yy48:
	++s;
#line 131 "re2c-scanner-c.re"
	{ return TOKEN_BACKSLASH; }
#line 621 "re2c-scanner-c.cc"
yy49:
	++s;
#line 68 "re2c-scanner-c.re"
	{ return TOKEN_R_BRACKET; }
#line 626 "re2c-scanner-c.cc"
yy50:
	yych = *++s;
	switch (yych) {
		case '=': goto yy102;
		default: goto yy51;
	}
yy51:
#line 100 "re2c-scanner-c.re"
	{ return TOKEN_POW; }
#line 636 "re2c-scanner-c.cc"
yy52:
	++s;
#line 64 "re2c-scanner-c.re"
	{ return TOKEN_L_BRACE; }
#line 641 "re2c-scanner-c.cc"
yy53:
	yych = *++s;
	switch (yych) {
		case '=': goto yy103;
		case '|': goto yy104;
		default: goto yy54;
	}
yy54:
#line 98 "re2c-scanner-c.re"
	{ return TOKEN_OR; }
#line 652 "re2c-scanner-c.cc"
yy55:
	++s;
#line 65 "re2c-scanner-c.re"
	{ return TOKEN_R_BRACE; }
#line 657 "re2c-scanner-c.cc"
yy56:
	yych = *++s;
	switch (yych) {
		case '=': goto yy105;
		default: goto yy57;
	}
yy57:
#line 87 "re2c-scanner-c.re"
	{ return TOKEN_TILDE; }
#line 667 "re2c-scanner-c.cc"
yy58:
	yych = *++s;
	switch (yych) {
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF: goto yy45;
		default: goto yy3;
	}
yy59:
	yyaccept = 0;
	yych = *(m = ++s);
	switch (yych) {
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF: goto yy99;
		default: goto yy3;
	}
yy60:
	yyaccept = 0;
	yych = *(m = ++s);
	switch (yych) {
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF: goto yy100;
		default: goto yy3;
	}
yy61:
	++s;
#line 84 "re2c-scanner-c.re"
	{ return TOKEN_NOT_EQUAL; }
#line 881 "re2c-scanner-c.cc"
yy62:
	++s;
	if (lim <= s) return false;
	yych = *s;
yy63:
	switch (yych) {
		case 0x00:
		case '\n': goto yy64;
		case '"': goto yy65;
		case '\\': goto yy67;
		default: goto yy62;
	}
yy64:
	s = m;
	switch (yyaccept) {
		case 0: goto yy3;
		case 1: goto yy33;
		case 2: goto yy46;
		case 3: goto yy83;
		default: goto yy66;
	}
yy65:
	++s;
yy66:
#line 54 "re2c-scanner-c.re"
	{ return TOKEN_STRING; }
#line 908 "re2c-scanner-c.cc"
yy67:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x00:
		case '\n': goto yy64;
		case '"': goto yy106;
		case '\\': goto yy67;
		default: goto yy62;
	}
yy68:
	++s;
#line 119 "re2c-scanner-c.re"
	{ return TOKEN_MOD_EQUAL; }
#line 924 "re2c-scanner-c.cc"
yy69:
	++s;
#line 97 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_AND; }
#line 929 "re2c-scanner-c.cc"
yy70:
	++s;
#line 111 "re2c-scanner-c.re"
	{ return TOKEN_AND_EQUAL; }
#line 934 "re2c-scanner-c.cc"
yy71:
	++s;
	if (lim <= s) return false;
	yych = *s;
yy72:
	switch (yych) {
		case 0x00:
		case '\n': goto yy64;
		case '\'': goto yy65;
		case '\\': goto yy73;
		default: goto yy71;
	}
yy73:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x00:
		case '\n': goto yy64;
		case '\'': goto yy107;
		case '\\': goto yy73;
		default: goto yy71;
	}
yy74:
	++s;
#line 128 "re2c-scanner-c.re"
	{ return TOKEN_C_COMMENT_END; }
#line 962 "re2c-scanner-c.cc"
yy75:
	++s;
#line 118 "re2c-scanner-c.re"
	{ return TOKEN_MULT_EQUAL; }
#line 967 "re2c-scanner-c.cc"
yy76:
	++s;
#line 102 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_PLUS; }
#line 972 "re2c-scanner-c.cc"
yy77:
	++s;
#line 115 "re2c-scanner-c.re"
	{ return TOKEN_PLUS_EQUAL; }
#line 977 "re2c-scanner-c.cc"
yy78:
	++s;
#line 103 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_MINUS; }
#line 982 "re2c-scanner-c.cc"
yy79:
	++s;
#line 116 "re2c-scanner-c.re"
	{ return TOKEN_MINUS_EQUAL; }
#line 987 "re2c-scanner-c.cc"
yy80:
	++s;
#line 72 "re2c-scanner-c.re"
	{ return TOKEN_MINUS_GREATER; }
#line 992 "re2c-scanner-c.cc"
yy81:
	yyaccept = 3;
	m = ++s;
	if ((lim - s) < 3) return false;
	yych = *s;
yy82:
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy81;
		case 'D':
		case 'E':
		case 'd':
		case 'e': goto yy87;
		case 'F':
		case 'L':
		case 'f':
		case 'l': goto yy88;
		default: goto yy83;
	}
yy83:
#line 53 "re2c-scanner-c.re"
	{ return TOKEN_FLOAT; }
#line 1023 "re2c-scanner-c.cc"
yy84:
	++s;
#line 127 "re2c-scanner-c.re"
	{ return TOKEN_C_COMMENT_START; }
#line 1028 "re2c-scanner-c.cc"
yy85:
	++s;
#line 126 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_DIV; }
#line 1033 "re2c-scanner-c.cc"
yy86:
	++s;
#line 117 "re2c-scanner-c.re"
	{ return TOKEN_DIV_EQUAL; }
#line 1038 "re2c-scanner-c.cc"
yy87:
	yych = *++s;
	switch (yych) {
		case '+':
		case '-': goto yy108;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy109;
		default: goto yy64;
	}
yy88:
	++s;
	goto yy83;
yy89:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 'L':
		case 'U':
		case 'l':
		case 'u': goto yy89;
		default: goto yy33;
	}
yy90:
	yych = *++s;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy110;
		default: goto yy64;
	}
yy91:
	++s;
#line 91 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_COLON; }
#line 1089 "re2c-scanner-c.cc"
yy92:
	yych = *++s;
	switch (yych) {
		case '=': goto yy111;
		default: goto yy93;
	}
yy93:
#line 74 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_LESS; }
#line 1099 "re2c-scanner-c.cc"
yy94:
	++s;
#line 80 "re2c-scanner-c.re"
	{ return TOKEN_LE; }
#line 1104 "re2c-scanner-c.cc"
yy95:
	++s;
#line 83 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_EQUAL; }
#line 1109 "re2c-scanner-c.cc"
yy96:
	++s;
#line 81 "re2c-scanner-c.re"
	{ return TOKEN_GE; }
#line 1114 "re2c-scanner-c.cc"
yy97:
	yych = *++s;
	switch (yych) {
		case '=': goto yy112;
		default: goto yy98;
	}
yy98:
#line 75 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_GREATER; }
#line 1124 "re2c-scanner-c.cc"
yy99:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF: goto yy45;
		default: goto yy64;
	}
yy100:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF: goto yy99;
		default: goto yy64;
	}
yy101:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
		case 0xA6:
		case 0xA7:
		case 0xA8:
		case 0xA9:
		case 0xAA:
		case 0xAB:
		case 0xAC:
		case 0xAD:
		case 0xAE:
		case 0xAF:
		case 0xB0:
		case 0xB1:
		case 0xB2:
		case 0xB3:
		case 0xB4:
		case 0xB5:
		case 0xB6:
		case 0xB7:
		case 0xB8:
		case 0xB9:
		case 0xBA:
		case 0xBB:
		case 0xBC:
		case 0xBD:
		case 0xBE:
		case 0xBF: goto yy100;
		default: goto yy64;
	}
yy102:
	++s;
#line 113 "re2c-scanner-c.re"
	{ return TOKEN_POW_EQUAL; }
#line 1342 "re2c-scanner-c.cc"
yy103:
	++s;
#line 112 "re2c-scanner-c.re"
	{ return TOKEN_OR_EQUAL; }
#line 1347 "re2c-scanner-c.cc"
yy104:
	++s;
#line 99 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_OR; }
#line 1352 "re2c-scanner-c.cc"
yy105:
	++s;
#line 85 "re2c-scanner-c.re"
	{ return TOKEN_NOT_EQUAL; }
#line 1357 "re2c-scanner-c.cc"
yy106:
	yyaccept = 4;
	m = ++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x00:
		case '\n': goto yy66;
		case '"': goto yy65;
		case '\\': goto yy67;
		default: goto yy62;
	}
yy107:
	yyaccept = 4;
	m = ++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case 0x00:
		case '\n': goto yy66;
		case '\'': goto yy65;
		case '\\': goto yy73;
		default: goto yy71;
	}
yy108:
	yych = *++s;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy109;
		default: goto yy64;
	}
yy109:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy109;
		case 'F':
		case 'L':
		case 'f':
		case 'l': goto yy88;
		default: goto yy83;
	}
yy110:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': goto yy110;
		case 'L':
		case 'U':
		case 'l':
		case 'u': goto yy89;
		default: goto yy33;
	}
yy111:
	++s;
#line 121 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_LESS_EQUAL; }
#line 1443 "re2c-scanner-c.cc"
yy112:
	++s;
#line 122 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_GREATER_EQUAL; }
#line 1448 "re2c-scanner-c.cc"
}
#line 137 "re2c-scanner-c.re"

}

/* Local Variables: */
/* mode: c */
/* coding: latin-1 */
/* End: */

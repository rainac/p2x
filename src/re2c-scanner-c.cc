/* Generated by re2c 1.1.1 on Fri Nov 16 20:25:22 2018 */
#line 1 "re2c-scanner-c.re"
/*
This file is part of P2X.
Copyright � 2018 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#include "token.ncd.enum.hh"
#include <string.h>

#define YYMAXFILL 4


typedef unsigned char uint8_t;

int re2c_scanner_c_lex(const uint8_t *&s, const uint8_t *&m, uint8_t const * lim)
{

  

  lim += YYMAXFILL;

    
#line 26 "re2c-scanner-c.cc"
{
	uint8_t yych;
	unsigned int yyaccept = 0;
	if ((lim - s) < 4) return false;
	yych = *s;
	switch (yych) {
	case 0x00:	goto yy2;
	case '\t':	goto yy6;
	case '\n':	goto yy8;
	case '\r':	goto yy10;
	case ' ':	goto yy12;
	case '!':	goto yy15;
	case '"':	goto yy17;
	case '#':	goto yy18;
	case '%':	goto yy20;
	case '&':	goto yy22;
	case '\'':	goto yy24;
	case '(':	goto yy25;
	case ')':	goto yy27;
	case '*':	goto yy29;
	case '+':	goto yy31;
	case ',':	goto yy33;
	case '-':	goto yy35;
	case '.':	goto yy37;
	case '/':	goto yy39;
	case '0':	goto yy41;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy43;
	case ':':	goto yy45;
	case ';':	goto yy47;
	case '<':	goto yy49;
	case '=':	goto yy51;
	case '>':	goto yy53;
	case '?':	goto yy55;
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
	case 'z':	goto yy57;
	case '[':	goto yy60;
	case ']':	goto yy62;
	case '^':	goto yy64;
	case '{':	goto yy66;
	case '|':	goto yy68;
	case '}':	goto yy70;
	case '~':	goto yy72;
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
	case 0xDF:	goto yy74;
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
	case 0xEF:	goto yy75;
	case 0xF0:
	case 0xF1:
	case 0xF2:
	case 0xF3:
	case 0xF4:
	case 0xF5:
	case 0xF6:
	case 0xF7:	goto yy76;
	default:	goto yy4;
	}
yy2:
	++s;
#line 133 "re2c-scanner-c.re"
	{ return TOKEN_EOF; }
#line 190 "re2c-scanner-c.cc"
yy4:
	++s;
yy5:
#line 135 "re2c-scanner-c.re"
	{ return TOKEN_ILLEGAL_CHAR; }
#line 196 "re2c-scanner-c.cc"
yy6:
	++s;
#line 61 "re2c-scanner-c.re"
	{ return TOKEN_TAB; }
#line 201 "re2c-scanner-c.cc"
yy8:
	++s;
#line 60 "re2c-scanner-c.re"
	{ return TOKEN_NEWLINE; }
#line 206 "re2c-scanner-c.cc"
yy10:
	++s;
#line 62 "re2c-scanner-c.re"
	{ return TOKEN_CRETURN; }
#line 211 "re2c-scanner-c.cc"
yy12:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
	case ' ':	goto yy12;
	default:	goto yy14;
	}
yy14:
#line 59 "re2c-scanner-c.re"
	{ return TOKEN_SPACE; }
#line 223 "re2c-scanner-c.cc"
yy15:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy77;
	default:	goto yy16;
	}
yy16:
#line 91 "re2c-scanner-c.re"
	{ return TOKEN_EXCLAM; }
#line 233 "re2c-scanner-c.cc"
yy17:
	yyaccept = 0;
	yych = *(m = ++s);
	switch (yych) {
	case 0x00:
	case '\n':	goto yy5;
	default:	goto yy80;
	}
yy18:
	++s;
#line 127 "re2c-scanner-c.re"
	{ return TOKEN_HASH; }
#line 246 "re2c-scanner-c.cc"
yy20:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy86;
	default:	goto yy21;
	}
yy21:
#line 112 "re2c-scanner-c.re"
	{ return TOKEN_MOD; }
#line 256 "re2c-scanner-c.cc"
yy22:
	yych = *++s;
	switch (yych) {
	case '&':	goto yy88;
	case '=':	goto yy90;
	default:	goto yy23;
	}
yy23:
#line 99 "re2c-scanner-c.re"
	{ return TOKEN_AND; }
#line 267 "re2c-scanner-c.cc"
yy24:
	yyaccept = 0;
	yych = *(m = ++s);
	switch (yych) {
	case 0x00:
	case '\n':	goto yy5;
	default:	goto yy93;
	}
yy25:
	++s;
#line 64 "re2c-scanner-c.re"
	{ return TOKEN_L_PAREN; }
#line 280 "re2c-scanner-c.cc"
yy27:
	++s;
#line 65 "re2c-scanner-c.re"
	{ return TOKEN_R_PAREN; }
#line 285 "re2c-scanner-c.cc"
yy29:
	yych = *++s;
	switch (yych) {
	case '/':	goto yy96;
	case '=':	goto yy98;
	default:	goto yy30;
	}
yy30:
#line 111 "re2c-scanner-c.re"
	{ return TOKEN_MULT; }
#line 296 "re2c-scanner-c.cc"
yy31:
	yych = *++s;
	switch (yych) {
	case '+':	goto yy100;
	case '=':	goto yy102;
	default:	goto yy32;
	}
yy32:
#line 108 "re2c-scanner-c.re"
	{ return TOKEN_PLUS; }
#line 307 "re2c-scanner-c.cc"
yy33:
	++s;
#line 95 "re2c-scanner-c.re"
	{ return TOKEN_COMMA; }
#line 312 "re2c-scanner-c.cc"
yy35:
	yych = *++s;
	switch (yych) {
	case '-':	goto yy104;
	case '=':	goto yy106;
	case '>':	goto yy108;
	default:	goto yy36;
	}
yy36:
#line 109 "re2c-scanner-c.re"
	{ return TOKEN_MINUS; }
#line 324 "re2c-scanner-c.cc"
yy37:
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
	case '9':	goto yy110;
	default:	goto yy38;
	}
yy38:
#line 97 "re2c-scanner-c.re"
	{ return TOKEN_FULL_STOP; }
#line 343 "re2c-scanner-c.cc"
yy39:
	yych = *++s;
	switch (yych) {
	case '*':	goto yy113;
	case '/':	goto yy115;
	case '=':	goto yy117;
	default:	goto yy40;
	}
yy40:
#line 110 "re2c-scanner-c.re"
	{ return TOKEN_DIV; }
#line 355 "re2c-scanner-c.cc"
yy41:
	yyaccept = 1;
	yych = *(m = ++s);
	switch (yych) {
	case '.':	goto yy110;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy43;
	case 'D':
	case 'E':
	case 'F':
	case 'd':
	case 'e':
	case 'f':	goto yy111;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy121;
	case 'x':	goto yy123;
	default:	goto yy42;
	}
yy42:
#line 55 "re2c-scanner-c.re"
	{ return TOKEN_INTEGER; }
#line 387 "re2c-scanner-c.cc"
yy43:
	yyaccept = 1;
	m = ++s;
	if ((lim - s) < 3) return false;
	yych = *s;
	switch (yych) {
	case '.':	goto yy110;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy43;
	case 'D':
	case 'E':
	case 'd':
	case 'e':	goto yy119;
	case 'F':
	case 'f':	goto yy120;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy121;
	default:	goto yy42;
	}
yy45:
	yych = *++s;
	switch (yych) {
	case ':':	goto yy124;
	default:	goto yy46;
	}
yy46:
#line 93 "re2c-scanner-c.re"
	{ return TOKEN_COLON; }
#line 426 "re2c-scanner-c.cc"
yy47:
	++s;
#line 96 "re2c-scanner-c.re"
	{ return TOKEN_SEMICOLON; }
#line 431 "re2c-scanner-c.cc"
yy49:
	yych = *++s;
	switch (yych) {
	case '<':	goto yy126;
	case '=':	goto yy128;
	default:	goto yy50;
	}
yy50:
#line 80 "re2c-scanner-c.re"
	{ return TOKEN_LESS; }
#line 442 "re2c-scanner-c.cc"
yy51:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy130;
	default:	goto yy52;
	}
yy52:
#line 73 "re2c-scanner-c.re"
	{ return TOKEN_EQUAL; }
#line 452 "re2c-scanner-c.cc"
yy53:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy132;
	case '>':	goto yy134;
	default:	goto yy54;
	}
yy54:
#line 81 "re2c-scanner-c.re"
	{ return TOKEN_GREATER; }
#line 463 "re2c-scanner-c.cc"
yy55:
	++s;
#line 92 "re2c-scanner-c.re"
	{ return TOKEN_QUESTION; }
#line 468 "re2c-scanner-c.cc"
yy57:
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
	case 'z':	goto yy57;
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
	case 0xDF:	goto yy136;
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
	case 0xEF:	goto yy137;
	case 0xF0:
	case 0xF1:
	case 0xF2:
	case 0xF3:
	case 0xF4:
	case 0xF5:
	case 0xF6:
	case 0xF7:	goto yy138;
	default:	goto yy59;
	}
yy59:
#line 54 "re2c-scanner-c.re"
	{ return TOKEN_IDENTIFIER; }
#line 599 "re2c-scanner-c.cc"
yy60:
	++s;
#line 70 "re2c-scanner-c.re"
	{ return TOKEN_L_BRACKET; }
#line 604 "re2c-scanner-c.cc"
yy62:
	++s;
#line 71 "re2c-scanner-c.re"
	{ return TOKEN_R_BRACKET; }
#line 609 "re2c-scanner-c.cc"
yy64:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy139;
	default:	goto yy65;
	}
yy65:
#line 103 "re2c-scanner-c.re"
	{ return TOKEN_POW; }
#line 619 "re2c-scanner-c.cc"
yy66:
	++s;
#line 67 "re2c-scanner-c.re"
	{ return TOKEN_L_BRACE; }
#line 624 "re2c-scanner-c.cc"
yy68:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy141;
	case '|':	goto yy143;
	default:	goto yy69;
	}
yy69:
#line 101 "re2c-scanner-c.re"
	{ return TOKEN_OR; }
#line 635 "re2c-scanner-c.cc"
yy70:
	++s;
#line 68 "re2c-scanner-c.re"
	{ return TOKEN_R_BRACE; }
#line 640 "re2c-scanner-c.cc"
yy72:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy145;
	default:	goto yy73;
	}
yy73:
#line 90 "re2c-scanner-c.re"
	{ return TOKEN_TILDE; }
#line 650 "re2c-scanner-c.cc"
yy74:
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
	case 0xBF:	goto yy57;
	default:	goto yy5;
	}
yy75:
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
	case 0xBF:	goto yy136;
	default:	goto yy5;
	}
yy76:
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
	case 0xBF:	goto yy137;
	default:	goto yy5;
	}
yy77:
	++s;
#line 87 "re2c-scanner-c.re"
	{ return TOKEN_NOT_EQUAL; }
#line 864 "re2c-scanner-c.cc"
yy79:
	++s;
	if (lim <= s) return false;
	yych = *s;
yy80:
	switch (yych) {
	case 0x00:
	case '\n':	goto yy81;
	case '"':	goto yy82;
	case '\\':	goto yy84;
	default:	goto yy79;
	}
yy81:
	s = m;
	switch (yyaccept) {
	case 0: 	goto yy5;
	case 1: 	goto yy42;
	case 2: 	goto yy59;
	case 3: 	goto yy112;
	default:	goto yy83;
	}
yy82:
	++s;
yy83:
#line 57 "re2c-scanner-c.re"
	{ return TOKEN_STRING; }
#line 891 "re2c-scanner-c.cc"
yy84:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
	case 0x00:
	case '\n':	goto yy81;
	case '"':	goto yy147;
	case '\\':	goto yy84;
	default:	goto yy79;
	}
yy86:
	++s;
#line 122 "re2c-scanner-c.re"
	{ return TOKEN_MOD_EQUAL; }
#line 907 "re2c-scanner-c.cc"
yy88:
	++s;
#line 100 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_AND; }
#line 912 "re2c-scanner-c.cc"
yy90:
	++s;
#line 114 "re2c-scanner-c.re"
	{ return TOKEN_AND_EQUAL; }
#line 917 "re2c-scanner-c.cc"
yy92:
	++s;
	if (lim <= s) return false;
	yych = *s;
yy93:
	switch (yych) {
	case 0x00:
	case '\n':	goto yy81;
	case '\'':	goto yy82;
	case '\\':	goto yy94;
	default:	goto yy92;
	}
yy94:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
	case 0x00:
	case '\n':	goto yy81;
	case '\'':	goto yy148;
	case '\\':	goto yy94;
	default:	goto yy92;
	}
yy96:
	++s;
#line 131 "re2c-scanner-c.re"
	{ return TOKEN_C_COMMENT_END; }
#line 945 "re2c-scanner-c.cc"
yy98:
	++s;
#line 121 "re2c-scanner-c.re"
	{ return TOKEN_MULT_EQUAL; }
#line 950 "re2c-scanner-c.cc"
yy100:
	++s;
#line 105 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_PLUS; }
#line 955 "re2c-scanner-c.cc"
yy102:
	++s;
#line 118 "re2c-scanner-c.re"
	{ return TOKEN_PLUS_EQUAL; }
#line 960 "re2c-scanner-c.cc"
yy104:
	++s;
#line 106 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_MINUS; }
#line 965 "re2c-scanner-c.cc"
yy106:
	++s;
#line 119 "re2c-scanner-c.re"
	{ return TOKEN_MINUS_EQUAL; }
#line 970 "re2c-scanner-c.cc"
yy108:
	++s;
#line 75 "re2c-scanner-c.re"
	{ return TOKEN_MINUS_GREATER; }
#line 975 "re2c-scanner-c.cc"
yy110:
	yyaccept = 3;
	m = ++s;
	if ((lim - s) < 3) return false;
	yych = *s;
yy111:
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
	case '9':	goto yy110;
	case 'D':
	case 'E':
	case 'd':
	case 'e':	goto yy119;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy120;
	default:	goto yy112;
	}
yy112:
#line 56 "re2c-scanner-c.re"
	{ return TOKEN_FLOAT; }
#line 1006 "re2c-scanner-c.cc"
yy113:
	++s;
#line 130 "re2c-scanner-c.re"
	{ return TOKEN_C_COMMENT_START; }
#line 1011 "re2c-scanner-c.cc"
yy115:
	++s;
#line 129 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_DIV; }
#line 1016 "re2c-scanner-c.cc"
yy117:
	++s;
#line 120 "re2c-scanner-c.re"
	{ return TOKEN_DIV_EQUAL; }
#line 1021 "re2c-scanner-c.cc"
yy119:
	yych = *++s;
	switch (yych) {
	case '+':
	case '-':	goto yy149;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy150;
	default:	goto yy81;
	}
yy120:
	++s;
	goto yy112;
yy121:
	++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy121;
	default:	goto yy42;
	}
yy123:
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
	case '9':	goto yy152;
	default:	goto yy81;
	}
yy124:
	++s;
#line 94 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_COLON; }
#line 1072 "re2c-scanner-c.cc"
yy126:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy154;
	default:	goto yy127;
	}
yy127:
#line 77 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_LESS; }
#line 1082 "re2c-scanner-c.cc"
yy128:
	++s;
#line 83 "re2c-scanner-c.re"
	{ return TOKEN_LE; }
#line 1087 "re2c-scanner-c.cc"
yy130:
	++s;
#line 86 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_EQUAL; }
#line 1092 "re2c-scanner-c.cc"
yy132:
	++s;
#line 84 "re2c-scanner-c.re"
	{ return TOKEN_GE; }
#line 1097 "re2c-scanner-c.cc"
yy134:
	yych = *++s;
	switch (yych) {
	case '=':	goto yy156;
	default:	goto yy135;
	}
yy135:
#line 78 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_GREATER; }
#line 1107 "re2c-scanner-c.cc"
yy136:
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
	case 0xBF:	goto yy57;
	default:	goto yy81;
	}
yy137:
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
	case 0xBF:	goto yy136;
	default:	goto yy81;
	}
yy138:
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
	case 0xBF:	goto yy137;
	default:	goto yy81;
	}
yy139:
	++s;
#line 116 "re2c-scanner-c.re"
	{ return TOKEN_POW_EQUAL; }
#line 1325 "re2c-scanner-c.cc"
yy141:
	++s;
#line 115 "re2c-scanner-c.re"
	{ return TOKEN_OR_EQUAL; }
#line 1330 "re2c-scanner-c.cc"
yy143:
	++s;
#line 102 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_OR; }
#line 1335 "re2c-scanner-c.cc"
yy145:
	++s;
#line 88 "re2c-scanner-c.re"
	{ return TOKEN_NOT_EQUAL; }
#line 1340 "re2c-scanner-c.cc"
yy147:
	yyaccept = 4;
	m = ++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
	case 0x00:
	case '\n':	goto yy83;
	case '"':	goto yy82;
	case '\\':	goto yy84;
	default:	goto yy79;
	}
yy148:
	yyaccept = 4;
	m = ++s;
	if (lim <= s) return false;
	yych = *s;
	switch (yych) {
	case 0x00:
	case '\n':	goto yy83;
	case '\'':	goto yy82;
	case '\\':	goto yy94;
	default:	goto yy92;
	}
yy149:
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
	case '9':	goto yy150;
	default:	goto yy81;
	}
yy150:
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
	case '9':	goto yy150;
	case 'F':
	case 'L':
	case 'f':
	case 'l':	goto yy120;
	default:	goto yy112;
	}
yy152:
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
	case '9':	goto yy152;
	case 'L':
	case 'U':
	case 'l':
	case 'u':	goto yy121;
	default:	goto yy42;
	}
yy154:
	++s;
#line 124 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_LESS_EQUAL; }
#line 1426 "re2c-scanner-c.cc"
yy156:
	++s;
#line 125 "re2c-scanner-c.re"
	{ return TOKEN_DOUBLE_GREATER_EQUAL; }
#line 1431 "re2c-scanner-c.cc"
}
#line 137 "re2c-scanner-c.re"

}

/* Local Variables: */
/* mode: c */
/* coding: latin-1 */
/* End: */

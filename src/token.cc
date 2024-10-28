/*
This file is part of P2X.
Copyright © 2013,2015 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#include <string>
#include <strings.h>
#include <stdio.h>
#include <stdint.h>
#include "token.hh"

Token::Token() :
  left(),
  right(),
  ignore(),
#ifdef P2X_SAVE_PROTO_PTR
  proto(),
#endif
  id(++count),
  line(),
  column(),
  character(),
  token(TOKEN_EOF)
{ }

Token::Token(ParserToken token, std::string text, int line, int column, int character) :
  text(text),
  left(),
  right(),
  ignore(),
#ifdef P2X_SAVE_PROTO_PTR
  proto(),
#endif
  id(++count),
  line(line), column(column), character(character), token(token)
{}

#ifdef DEBUG
Token::~Token() {
  left = right = 0;
  ignore = 0;
  token = TOKEN_EOF;
}
#endif

int Token::count;

/* 
This file is part of P2X.
Copyright © 2013,2015 Johannes Willkomm
See the file p2x.cc for copying conditions.  
*/

#include <string>
#include <strings.h>
#include <stdio.h>
#include "token.hh"

Token::Token() : 
  left(), 
  right(),
  ignore(), 
  content(), 
  rmt(),
  id(++count),
  line(), 
  column(),
  character(), 
  token(TOKEN_EOF)
{ }

Token::Token(ParserToken token, std::string text, int line, int column, int character) : 
  text(text), left(), right(), ignore(), content(), id(++count), line(line), column(column), character(character), token(token)
{}

Token::~Token() {
  left = right = 0;
  content = ignore = 0;
  rmt = 0;
  token = TOKEN_EOF;
}


int Token::count = 0;

// $Id$

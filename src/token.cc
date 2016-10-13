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
  rmt(),
  id(++count),
  line(), 
  column(),
  character(), 
  flags(),
  token(TOKEN_EOF)
{ }

Token::Token(ParserToken token, std::string text, int line, int column, int character) : 
  text(text), left(), right(), ignore(), id(++count), line(line), column(column), character(character), flags(), token(token)
{}

Token::~Token() {
  left = right = 0;
  ignore = 0;
  rmt = 0;
  token = TOKEN_EOF;
}


int Token::count = 0;

// $Id$

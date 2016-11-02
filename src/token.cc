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
  id(++count),
  line(), 
  column(),
  character(), 
  token(TOKEN_EOF)
{ }

Token::Token(ParserToken token, std::string text, int line, int column, int character) : 
  text(text), left(), right(), ignore(), id(++count), line(line), column(column), character(character), token(token)
{}

Token::~Token() {
  left = right = 0;
  ignore = 0;
  token = TOKEN_EOF;
}


int Token::count;

/* 
This file is part of P2X.
Copyright © 2013 Johannes Willkomm 
See the file p2x.cc for copying conditions.  
*/

#include <strings.h>
#include <iostream>
#include <string>
#include "token.ncd.enum.hh"

struct Token {

  std::string text;

  Token *left, *right, *ignore, *content;

  int id, line, column, character;

  ParserToken token;

  static int count;

  Token();
  Token(ParserToken token, std::string text, int line = 0, int column = 0, int character = 0);
  ~Token();

  void print(std::ostream &aus) const {
    aus << "Token(" << id << "," << token << "(" << Token::getParserTokenName(token) << "),'" << text << "'"
        << "," << line << "," << column << "," << character << ")";
  }

// #include "token.ncd.enum.hh"
#include "token.ncd.cc"

};

#include "token.ncd.hh"

inline std::ostream &operator<<(std::ostream &aus, Token const &t) {
  t.print(aus);
  return aus;
}

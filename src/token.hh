/* 
This file is part of P2X.
Copyright © 2013,2015,2016 Johannes Willkomm
See the file p2x.cc for copying conditions.  
*/

#include <strings.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include "token.ncd.enum.hh"

struct Token {

  std::string text;

  Token *left, *right, *ignore;

  int id, line, column, character;

  ParserToken token;

  static int count;

  Token();
  Token(ParserToken token, std::string text, int line = 0, int column = 0, int character = 0);
#ifdef DEBUG
  ~Token();
#endif

  char const *name() const { return getParserTokenName(token); }

  void prints(std::ostream &aus) const {
    aus << "'" << text << "'[" << name() << "," << line << "," << column << "]";
  }

  void print(std::ostream &aus) const {
    aus << "Token(" << id << "," << token << "(" << name() << "),'" << text << "'"
        << "," << line << "," << column << "," << character << ")";
  }

  //#include "token.ncd.enum.hh"
#include "token.ncd.cc"
};

#include "token.ncd.hh"

inline std::ostream &operator<<(std::ostream &aus, Token const &t) {
  t.prints(aus);
  return aus;
}

inline std::ostream &operator<(std::ostream &aus, Token const &t) {
  t.print(aus);
  return aus;
}

struct TextLineAndCol {
  Token const &m_t;
  TextLineAndCol(Token const &t) : m_t(t) {}
  TextLineAndCol(Token const *t) : m_t(*t) {}
};

inline std::ostream &operator <<(std::ostream &aus, TextLineAndCol const &t) {
  aus << "'" << t.m_t.text << "' @" << t.m_t.line << ":" << t.m_t.column;
  return aus;
}

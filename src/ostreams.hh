/*
This file is part of P2X.
Copyright © 2016 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#pragma once

#include <iostream>
#include <stdio.h>

struct StreamBufferMATLABEscape : public std::streambuf {

  std::streambuf *m_sb1;
  bool escapeQuote;

  StreamBufferMATLABEscape(std::streambuf *sb1) :
    m_sb1(sb1),
    escapeQuote()
  {}

  int overflow(int c) {
    if (c == EOF or not m_sb1) {
      return 0;
    }
    int r1 = 0;
    switch(c) {
    case '\'':
      m_sb1->sputc('\'');
      r1 = m_sb1->sputc('\'');
      break;
    case '\n':
      m_sb1->sputc('\'');
      xsputn(", char(10), ", 12);
      m_sb1->sputc('\'');
      break;
    case '\r':
      m_sb1->sputc('\'');
      xsputn(", char(13), ", 12);
      m_sb1->sputc('\'');
      break;
    default:
      r1 = m_sb1->sputc(c);
      break;
    }
    return r1 == EOF ? EOF : c;
  }

  int sync() {
    int const r1 = m_sb1->pubsync();
    return r1 == 0 ? 0 : -1;
  }

private:
  StreamBufferMATLABEscape(StreamBufferMATLABEscape const &) : std::streambuf(), m_sb1() {}
  StreamBufferMATLABEscape &operator =(StreamBufferMATLABEscape const &) {
    return *this;
  }
};

struct OstreamMATLABEscape : public std::ostream {
  StreamBufferMATLABEscape tbuf;

  OstreamMATLABEscape(std::ostream &o1) :
    tbuf(o1.rdbuf())
  {
    rdbuf(&tbuf);
  }

};

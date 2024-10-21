/*
This file is part of P2X.
Copyright © 2013 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#ifndef jw_util_teestream_hh
#define jw_util_teestream_hh
#include <iostream>
#include <stdio.h>

//
// $Id: xmlOstream.hh 3231 2011-04-19 16:48:54Z willkomm $
//
// see http://wordaligned.org/articles/cpp-streambufs
//

struct XMLBuf : public std::streambuf {

  std::streambuf *m_sb1;

  bool escapeQuote;

  XMLBuf(std::streambuf *sb1) :
    m_sb1(sb1),
    escapeQuote()
  {}

  int overflow(int c) {
    if (c == EOF or not m_sb1) {
      return 0;
    }
    int r1 = 0;
    switch(c) {
    case '<':
      m_sb1->sputc('&');
      m_sb1->sputc('l');
      m_sb1->sputc('t');
      r1 = m_sb1->sputc(';');
      break;
    case '>':
      m_sb1->sputc('&');
      m_sb1->sputc('g');
      m_sb1->sputc('t');
      r1 = m_sb1->sputc(';');
      break;
    case '&':
      m_sb1->sputc('&');
      m_sb1->sputc('a');
      m_sb1->sputc('m');
      m_sb1->sputc('p');
      r1 = m_sb1->sputc(';');
      break;
    case '\"':
      if (escapeQuote) {
        m_sb1->sputc('&');
        m_sb1->sputc('q');
        m_sb1->sputc('u');
        m_sb1->sputc('o');
        m_sb1->sputc('t');
        r1 = m_sb1->sputc(';');
        break;
      }
      // fallthrough
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
  XMLBuf(XMLBuf const &) : std::streambuf(), m_sb1() {}
  XMLBuf &operator =(XMLBuf const &) {
    return *this;
  }
};

struct XMLOstream : public std::ostream {
  XMLBuf tbuf;

  XMLOstream(std::ostream &o1) :
    tbuf(o1.rdbuf())
  {
    rdbuf(&tbuf);
  }

};

#endif

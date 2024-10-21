/*
This file is part of P2X.
Copyright © 2013 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#ifndef logger_idp640_hh
#define logger_idp640_hh

#include <map>
#include <iostream>
#include <functional>
#include <strings.h>
#include <stdio.h>

struct LS {
#ifdef DEBUG
#warning DEBUG defined, undefining
#undef DEBUG
#endif
#include "logger.ncd.enum.hh"
#include "logger.ncd.cc"

  static int mask;
  typedef std::map<int, std::ostream*> SMap;
  static SMap m_map;
  static void setStream(int const code, std::ostream &aus) {
    m_map[code] = &aus;
  }
  struct NullStream : public std::ostream {
    NullStream() : std::ostream(0) {};
    NullStream &put(char) {return *this;}
    NullStream &write(const char *, std::streamsize) {return *this;}
  };
  static NullStream nullStream;
  static std::ostream &select(int const code) {
    SMap::const_iterator it = m_map.find(code);
    if (it != m_map.end()) {
      return *it->second;
    }
    return std::cerr;
  }
};

#ifdef __WIN32
int ffs(int x);
#endif

inline std::ostream &getls(int const code = 0xffffffff) {
  if (code & LS::mask) {
    std::ostream &res = LS::select(1 << ffs(code));
    res << "p2x: ";
    return res;
  }
  return LS::nullStream;
}

inline void logmsg(int const code, std::function<void()> func) {
  if (code & LS::mask) {
    func();
  }
}

#define Log(c, x)  logmsg((c), [&]() { getls() << x; });

#define IfLog(c, x)  if ((c) & LS::mask) { x }

#include "logger.ncd.hh"

#endif

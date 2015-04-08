/* 
This file is part of P2X.
Copyright © 2013 Johannes Willkomm 
See the file p2x.cc for copying conditions.  
*/

#ifndef logger_idp640_hh
#define logger_idp640_hh

#include <map>
#include <iostream>
#include <strings.h>

struct LS {
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

inline std::ostream &ls(int const code) {
  if (code & LS::mask) {
    std::ostream &res = LS::select(1 << ffs(code));
    res << "p2x: ";
    return res;
  }
  return LS::nullStream;
}

#include "logger.ncd.hh"

#endif

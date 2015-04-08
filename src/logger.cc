/* 
This file is part of P2X.
Copyright © 2013,2014 Johannes Willkomm 
See the file p2x.cc for copying conditions.  
*/

#include "logger.hh"

LS::NullStream LS::nullStream;
LS::SMap LS::m_map;
int LS::mask = LS::ERROR | LS::WARNING;

#ifdef __WIN32
int ffs(int x) {
  int m = 0x1;
  int k = 1;
  while((x & m) == 0) {
    m <<= 1;
    k += 1;
  }
  return (x & m) ? k : 0;
}
#endif

// $Id$

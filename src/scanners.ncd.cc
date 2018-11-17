// -*- c++ -*- 
// This file has been automatically generated by
// gennc.sh $Id: gennc.xsl 110 2018-10-09 15:02:12Z jwillkomm $
// from definition file xml/scanners.ncd.xml. Class mode is false.

#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "scanners.ncd.enum.hh"
#include "scanners.ncd.hh"
    char const *getScannerTypeName(unsigned long c) {
      switch(c) { 
       case SCANNER_NONE: return "NONE";
       case SCANNER_STRINGS: return "STRINGS";
       case SCANNER_NO_STRINGS: return "NO_STRINGS";
       case SCANNER_R: return "R";
       case SCANNER_M: return "M";
       case SCANNER_C: return "C";
       case SCANNER_FLEX_STRINGS: return "FLEX_STRINGS";
       case SCANNER_FLEX_NO_STRINGS: return "FLEX_NO_STRINGS";
       case SCANNER_FLEX_R: return "FLEX_R";
       case SCANNER_FLEX_M: return "FLEX_M";
       case SCANNER_FLEX_C: return "FLEX_C";
       case SCANNER_RE2C_STRINGS: return "RE2C_STRINGS";
       case SCANNER_RE2C_R: return "RE2C_R";
       case SCANNER_RE2C_M: return "RE2C_M";
       case SCANNER_RE2C_C: return "RE2C_C";
        default: break;
      }
      return "unknown value in enumeration ScannerType";
    }
    char const *getScannerTypeComment(unsigned long c) {
      switch(c) { 
       case SCANNER_NONE: return "";
       case SCANNER_STRINGS: return "";
       case SCANNER_NO_STRINGS: return "";
       case SCANNER_R: return "";
       case SCANNER_M: return "";
       case SCANNER_C: return "";
       case SCANNER_FLEX_STRINGS: return "";
       case SCANNER_FLEX_NO_STRINGS: return "";
       case SCANNER_FLEX_R: return "";
       case SCANNER_FLEX_M: return "";
       case SCANNER_FLEX_C: return "";
       case SCANNER_RE2C_STRINGS: return "";
       case SCANNER_RE2C_R: return "";
       case SCANNER_RE2C_M: return "";
       case SCANNER_RE2C_C: return "";
        default: break;
      }
      return "unknown value in enumeration ScannerType";
    }
    ScannerType getScannerTypeValue(char const *name, int *res) {
       if (res) *res = 0;
       if (name == 0 || *name == 0) { return SCANNER_NONE;
       } else if (strcasecmp("NONE", name) == 0
          || strcasecmp("SCANNER_NONE", name) == 0) {
          return SCANNER_NONE;
       } else if (strcasecmp("STRINGS", name) == 0
          || strcasecmp("SCANNER_STRINGS", name) == 0) {
          return SCANNER_STRINGS;
       } else if (strcasecmp("NO_STRINGS", name) == 0
          || strcasecmp("SCANNER_NO_STRINGS", name) == 0) {
          return SCANNER_NO_STRINGS;
       } else if (strcasecmp("R", name) == 0
          || strcasecmp("SCANNER_R", name) == 0) {
          return SCANNER_R;
       } else if (strcasecmp("M", name) == 0
          || strcasecmp("SCANNER_M", name) == 0) {
          return SCANNER_M;
       } else if (strcasecmp("C", name) == 0
          || strcasecmp("SCANNER_C", name) == 0) {
          return SCANNER_C;
       } else if (strcasecmp("FLEX_STRINGS", name) == 0
          || strcasecmp("SCANNER_FLEX_STRINGS", name) == 0) {
          return SCANNER_FLEX_STRINGS;
       } else if (strcasecmp("FLEX_NO_STRINGS", name) == 0
          || strcasecmp("SCANNER_FLEX_NO_STRINGS", name) == 0) {
          return SCANNER_FLEX_NO_STRINGS;
       } else if (strcasecmp("FLEX_R", name) == 0
          || strcasecmp("SCANNER_FLEX_R", name) == 0) {
          return SCANNER_FLEX_R;
       } else if (strcasecmp("FLEX_M", name) == 0
          || strcasecmp("SCANNER_FLEX_M", name) == 0) {
          return SCANNER_FLEX_M;
       } else if (strcasecmp("FLEX_C", name) == 0
          || strcasecmp("SCANNER_FLEX_C", name) == 0) {
          return SCANNER_FLEX_C;
       } else if (strcasecmp("RE2C_STRINGS", name) == 0
          || strcasecmp("SCANNER_RE2C_STRINGS", name) == 0) {
          return SCANNER_RE2C_STRINGS;
       } else if (strcasecmp("RE2C_R", name) == 0
          || strcasecmp("SCANNER_RE2C_R", name) == 0) {
          return SCANNER_RE2C_R;
       } else if (strcasecmp("RE2C_M", name) == 0
          || strcasecmp("SCANNER_RE2C_M", name) == 0) {
          return SCANNER_RE2C_M;
       } else if (strcasecmp("RE2C_C", name) == 0
          || strcasecmp("SCANNER_RE2C_C", name) == 0) {
          return SCANNER_RE2C_C;
       } else {
          if (! res) {
            fprintf(stderr, "error: unknown %s constant named `%s'\n",
              "ScannerType", name);
          } else {
             *res = 1;
          }
          return SCANNER_NONE;
       }
    }
    unsigned long getNumScannerType() {
      return 15;
    }
    ScannerType getScannerType(int which) {
      switch(which) { 
       case 0: return SCANNER_NONE;
       case 1: return SCANNER_STRINGS;
       case 2: return SCANNER_NO_STRINGS;
       case 3: return SCANNER_R;
       case 4: return SCANNER_M;
       case 5: return SCANNER_C;
       case 6: return SCANNER_FLEX_STRINGS;
       case 7: return SCANNER_FLEX_NO_STRINGS;
       case 8: return SCANNER_FLEX_R;
       case 9: return SCANNER_FLEX_M;
       case 10: return SCANNER_FLEX_C;
       case 11: return SCANNER_RE2C_STRINGS;
       case 12: return SCANNER_RE2C_R;
       case 13: return SCANNER_RE2C_M;
       case 14: return SCANNER_RE2C_C;
       default: break;
      }
      fprintf(stderr, "error: ScannerType constant index %d out of range\n",
              which);
      return SCANNER_NONE;
    }
    

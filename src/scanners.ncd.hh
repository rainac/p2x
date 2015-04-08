// -*- c++ -*- 
// This file has been automatically generated by 
// gennc.sh $Id: gennc.xsl 31 2013-11-07 18:56:43Z jwillkomm $ 
// from definition file xml/scanners.ncd.xml. Class mode is false.
#ifndef gennc_funcScannerType__hh
#define gennc_funcScannerType__hh

    char const *getScannerTypeName(unsigned long c);
    char const *getScannerTypeComment(unsigned long c);
    ScannerType getScannerTypeValue(char const *name, int *res = 0);
    inline ScannerType getScannerTypeValue(std::string const &t, int *res = 0) {
       return getScannerTypeValue(t.c_str(), res);
    }
    unsigned long getNumScannerType();
    ScannerType getScannerType(int which);
    
    inline std::ostream &operator << (std::ostream &out, ScannerType t) {
       out << getScannerTypeName(t);
       return out;
    }
    #endif


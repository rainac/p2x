// -*- c++ -*- 
// This file has been automatically generated by
// gennc.sh $Id: gennc.xsl 109 2018-10-08 16:10:40Z jwillkomm $
// from definition file xml/token.ncd.xml. Class mode is true.
#ifndef gennc_funcParserToken__hh
#define gennc_funcParserToken__hh

    inline std::ostream &operator << (std::ostream &out, ParserToken t) {
       out << Token::getParserTokenName(t);
       return out;
    }
    #endif


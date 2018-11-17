
    if (typeof window == 'undefined') {
      var P2X = require('./scanner.js');
    }
    var scanner = P2X.scanner;
    
  
    /* 
    This file is part of P2X.
    Copyright © 2013 Johannes Willkomm 
    See the file p2x.cc for copying conditions.  
    */

    
  
    scanner.add(/((0x)?([0-9])+[uUlL]*)/, TOKEN_INTEGER)
  
    scanner.add(/((\""\[\^\"\n\]\*"\")|('"\[\^\'\n\]\*"'))/, TOKEN_STRING)
  
    scanner.add(/(((([0-9])+\.([0-9])+)|(([0-9])+(\.([0-9])*)?)|(([0-9])*\.([0-9])+))(([eEdD][+-]?([0-9])+)?[lLfF]?))/, TOKEN_FLOAT)
  
    scanner.add(/\(/, TOKEN_L_PAREN)
  
    scanner.add(/\)/, TOKEN_R_PAREN)
  
    scanner.add(/{/, TOKEN_L_BRACE)
  
    scanner.add(/}/, TOKEN_R_BRACE)
  
    scanner.add(/\[/, TOKEN_L_BRACKET)
  
    scanner.add(/\]/, TOKEN_R_BRACKET)
  
    scanner.add(/=/, TOKEN_EQUAL)
  
    scanner.add(/</, TOKEN_LESS)
  
    scanner.add(/>/, TOKEN_GREATER)
  
    scanner.add(/~/, TOKEN_TILDE)
  
    scanner.add(/!/, TOKEN_EXCLAM)
  
    scanner.add(/\?/, TOKEN_QUESTION)
  
    scanner.add(/:/, TOKEN_COLON)
  
    scanner.add(/,/, TOKEN_COMMA)
  
    scanner.add(/;/, TOKEN_SEMICOLON)
  
    scanner.add(/\./, TOKEN_FULL_STOP)
  
    scanner.add(/@/, TOKEN_AT)
  
    scanner.add(/&/, TOKEN_AND)
  
    scanner.add(/\|/, TOKEN_OR)
  
    scanner.add(/\+/, TOKEN_PLUS)
  
    scanner.add(/-/, TOKEN_MINUS)
  
    scanner.add(/\//, TOKEN_DIV)
  
    scanner.add(/\\/, TOKEN_BACKSLASH)
  
    scanner.add(/\*/, TOKEN_MULT)
  
    scanner.add(/%/, TOKEN_MOD)
  
    scanner.add(/\^/, TOKEN_POW)
  
    scanner.add(/_/, TOKEN_UNDERSCORE)
  
    scanner.add(/$/, TOKEN_DOLLAR)
  
    scanner.add(/°/, TOKEN_CIRC)
  
    scanner.add(/#/, TOKEN_HASH)
  
    scanner.add(/\"/, TOKEN_QUOTE)
  
    scanner.add(/'/, TOKEN_APOS)
  
    scanner.add(/(\r)/, TOKEN_CRETURN)
  
    scanner.add(/(\n)/, TOKEN_NEWLINE)
  
    scanner.add(/(\t)/, TOKEN_TAB)
  
    scanner.add(/( )+/, TOKEN_SPACE)
  
    scanner.add(/(((([a-z])|([A-Z])|([\u0100-\uffff])))((([a-z])|([A-Z])|([\u0100-\uffff]))|([0-9])|(_))*)/, TOKEN_IDENTIFIER)
  
    scanner.add(/./, TOKEN_ILLEGAL_CHAR)
  

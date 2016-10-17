/*

This file is part of P2X.

P2X is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LPGL) as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

P2X is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public
License along with P2X.  If not, see <http://www.gnu.org/licenses/>.

Diese Datei ist Teil von P2X.

P2X ist Freie Software: Sie können es unter den Bedingungen der GNU
Lesser General Public License, wie von der Free Software Foundation,
Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
veröffentlichten Version, weiterverbreiten und/oder modifizieren.

P2X wird in der Hoffnung, dass es nützlich sein wird, aber OHNE JEDE
GEWÄHELEISTUNG, bereitgestellt; sogar ohne die implizite
Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN
ZWECK.  Siehe die GNU Lesser General Public License für weitere
Details.

Sie sollten eine Kopie der GNU Lesser General Public License zusammen
mit diesem Programm erhalten haben. Wenn nicht, siehe
<http://www.gnu.org/licenses/>.

Copyright (C) 2011-2016 Johannes Willkomm

*/

#undef  yyFlexLexer
#define yyFlexLexer scanFlexLexer
#include <FlexLexer.h>

#undef  yyFlexLexer
#define yyFlexLexer scanNSFlexLexer
#include <FlexLexer.h>

#undef  yyFlexLexer
#define yyFlexLexer scanRFlexLexer
#include <FlexLexer.h>

#undef  yyFlexLexer
#define yyFlexLexer scanCFlexLexer
#include <FlexLexer.h>

#undef  yyFlexLexer

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <limits>
#include <functional>
#include <list>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#ifndef __WIN32
#include <sys/time.h>
#endif
#include "config.h"
#include "token.hh"
#include "xmlOstream.hh"
#include "modes.ncd.enum.hh"
#include "modes.ncd.hh"
#include "modes.ncd.cc"
#include "assoc.ncd.enum.hh"
#include "assoc.ncd.hh"
#include "assoc.ncd.cc"
#include "scanners.ncd.enum.hh"
#include "scanners.ncd.hh"
#include "scanners.ncd.cc"
#include "output-modes.ncd.enum.hh"
#include "output-modes.ncd.hh"
#include "output-modes.ncd.cc"
#include "logger.hh"
#include "p2x-opts.hh"
#include "namespaces.hh"
#include "ostreams.hh"

#ifdef __WIN32
#include <windows.h>
#include <shlobj.h>
#undef ERROR

std::string winGetFolderPath() {
  std::string result;
  TCHAR szPath[MAX_PATH];
  HRESULT res = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
  if (SUCCEEDED(res)) {
    result = szPath;
    ls(LS::INFO) << "GetFolderPath(APPDATA): " << result << "\n";
  } else {
    ls(LS::ERROR) << "Failed to GetFolderPath(APPDATA)\n";
  }
  return result;
}
#endif

#ifdef __WIN32
// double getSecs() { return time(0); }
double getSecs() { return GetTickCount() * 1e-3; }
#else
double getSecs() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}
#endif

struct Timer {
  double m_start;
  Timer() : m_start(getSecs()) {}
  void reset() { m_start = getSecs(); }
  operator double() const { return getSecs() - m_start; }
  double get() const { return getSecs() - m_start; }
};

struct GetEnvString {
  std::string value;
  bool present;
  GetEnvString(std::string const &vname, std::string const &def = "") :
    value(def),
    present()
  {
    char const *envString = ::getenv(vname.c_str());
    if (envString) {
      present = 1;
      value = envString;
    }
  }

  operator std::string const &() const { return this->value; }
  std::string const &operator()() const { return this->value; }

  operator bool() const { return this->good(); }
  bool good() const { return this->present; }
  bool fail() const { return !this->good(); }
};

struct TokenList {
  size_t offset;
  std::vector<Token*> &tokenList;

  TokenList(std::vector<Token*> &tokenList) : offset(), tokenList(tokenList) {}

  void reset() { offset = 0; }
  Token *next() {
    return tokenList[offset++];
  }
};

// enum ParserMode {
//   MODE_IGNORE,
//   MODE_PAREN,
//   MODE_ITEM,
//   MODE_UNARY,
//   MODE_BINARY
// };

// enum OutputMode {
//   OUTPUT_MODE_NONE = 0,
//   OUTPUT_MODE_NESTED,
//   OUTPUT_MODE_COLLECT_DESCENDANTS
// };

struct TokenProto : public Token {
  int precedence, unaryPrecedence;
  ParserMode mode;
  bool isParen;
  bool isRParen;
  ParserAssoc associativity;
  OutputMode outputMode;
  typedef std::map<unsigned, TokenProto> EndList;
  EndList endList;
  TokenProto() :
    precedence(),
    unaryPrecedence(),
    mode(),
    isParen(),
    isRParen(),
    associativity(),
    outputMode()
  {}
  TokenProto(Token const &t, int precedence = 0,
             ParserMode mode = MODE_BINARY, ParserAssoc associativity = ASSOC_NONE,
             EndList const &endList = EndList(), int unaryPrecedence = 0) :
    Token(t),
    precedence(precedence),
    unaryPrecedence(unaryPrecedence),
    mode(mode),
    isParen(),
    isRParen(),
    associativity(associativity),
    outputMode(),
    endList(endList)
  {}
  void print(std::ostream &aus) const;
};
inline std::ostream &operator<<(std::ostream &aus, TokenProto const &t) {
  t.print(aus);
  return aus;
}

typedef TokenProto::EndList EndList;
std::ostream &operator << (std::ostream &out, EndList const &endList) {
  out << "{";
  for (EndList::const_iterator it = endList.begin(); it != endList.end(); ++it) {
    if (it != endList.begin()) {
      out << ", ";
    }
    out << it->first << "(" << it->second << ")";
  }
  out << "}";
  return out;
}

void TokenProto::print(std::ostream &aus) const {
  aus << "TokenProto(";
  Token::print(aus);
  aus << "," << precedence << "," << mode << "," << associativity << ", " << endList << ")";
}

struct TokenInfo {
  typedef std::map<ParserToken, TokenProto> Prototypes;
  Prototypes prototypes;
  typedef std::map<std::string, unsigned> OpCodes;
  OpCodes opCodes;
  typedef std::map<unsigned, TokenProto> OpPrototypes;
  OpPrototypes opPrototypes;

  TokenInfo() {
    init();
  }

  void init() {
    initMandatory();
    // initConvenient();
  }

  void initMandatory() {
    prototypes[TOKEN_JUXTA] = TokenProto(Token(TOKEN_JUXTA, "j"), 900, MODE_BINARY, ASSOC_LEFT);
  }

  void initConvenient() {
    initBraces();
    prototypes[TOKEN_SPACE] = TokenProto(Token(TOKEN_SPACE, "*"), 0, MODE_IGNORE);
  }

  void initBraces() {
    EndList endListP;
    endListP.insert(std::make_pair(unsigned(TOKEN_R_PAREN), Token(TOKEN_R_PAREN, ")")));
    prototypes[TOKEN_L_PAREN] = TokenProto(Token(TOKEN_L_PAREN, "*"), 10000, MODE_PAREN, ASSOC_NONE, endListP);
    EndList endListCB;
    endListCB.insert(std::make_pair(unsigned(TOKEN_R_BRACE), Token(TOKEN_R_BRACE, "}")));
    prototypes[TOKEN_L_BRACE] = TokenProto(Token(TOKEN_L_BRACE, "*"), 10000, MODE_PAREN, ASSOC_NONE, endListCB);
    EndList endListB;
    endListB.insert(std::make_pair(unsigned(TOKEN_R_BRACKET), Token(TOKEN_R_BRACKET, "]")));
    prototypes[TOKEN_L_BRACKET] = TokenProto(Token(TOKEN_L_BRACKET, "*"), 10000, MODE_PAREN, ASSOC_NONE, endListB);
  }

  unsigned mkOpCode(std::string const &s) {
    OpCodes::const_iterator it = opCodes.find(s);
    if (it != opCodes.end()) {
      return it->second;
    } else {
      unsigned const newCode = ceil((TOKEN_LAST + 1)/100.) * 100 + opCodes.size();
      ls(LS::INFO) << "Generating new opcode for named operator: " << s << " -> " << newCode << "\n";
      opCodes[s] = newCode;
      return newCode;
    }
  }

  unsigned mkOpCode(TokenProto const &tp) {
    if (tp.token == TOKEN_IDENTIFIER) {
      return mkOpCode(tp.text);
    } else {
      return tp.token;
    }
  }

  // unsigned mkOpCode(ParserToken t) {
  //   return t;
  // }
  // unsigned mkOpCode(Token const * const t) {
  //   return mkOpCode(t->text);
  // }

  unsigned getOpCode(std::string const &s) const {
    OpCodes::const_iterator it = opCodes.find(s);
    if (it != opCodes.end()) {
      return it->second;
    }
    return TOKEN_IDENTIFIER;
  }
  unsigned getOpCode(Token const * const t) const {
    if (t->token == TOKEN_IDENTIFIER) {
      return getOpCode(t->text);
    } else {
      return t->token;
    }
  }

  bool addBinary(std::string const &s, int prec, ParserAssoc assoc = ASSOC_LEFT) {
    if (prec <= 1) {
      ls(LS::ERROR|LS::PARSE) << "Parser: error: precedence must be > 1\n";
      prec = 2;
    }
    TokenProto p(Token(TOKEN_IDENTIFIER, s), prec, MODE_BINARY, assoc);
    ls(LS::CONFIG|LS::INFO) << "Adding binary named operator: " << p << "\n";
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addBinary(ParserToken t, int prec, ParserAssoc assoc = ASSOC_LEFT) {
    if (prec <= 1) {
      ls(LS::ERROR|LS::PARSE) << "Parser: error: precedence must be > 1\n";
      prec = 2;
    }
    TokenProto p(Token(t, "unknown"), prec, MODE_BINARY, assoc);
    ls(LS::CONFIG|LS::INFO) << "Adding binary operator: " << p << "\n";
    prototypes[t] = p;
    return true;
  }

  bool addUnary(std::string const &s, int prec) {
    if (prec <= 1) {
      ls(LS::ERROR|LS::PARSE) << "Parser: error: precedence must be > 1\n";
      prec = 2;
    }
    TokenProto p(Token(TOKEN_IDENTIFIER, s), prec, MODE_UNARY);
    ls(LS::CONFIG|LS::INFO) << "Adding unary named operator: " << p << "\n";
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addUnary(ParserToken t, int prec) {
    if (prec <= 1) {
      ls(LS::ERROR|LS::PARSE) << "Parser: error: precedence must be > 1\n";
      prec = 2;
    }
    TokenProto p(Token(t, "unknown"), prec, MODE_UNARY);
    ls(LS::CONFIG|LS::INFO) << "Adding unary operator: " << p << "\n";
    if (t == TOKEN_JUXTA) {
      ls(LS::ERROR|LS::PARSE) << "error: Parser: cannot classify JUXTA as Unary\n";
    }
    prototypes[t] = p;
    return true;
  }

  bool addPostfix(std::string const &s, int prec) {
    if (prec <= 1) {
      ls(LS::ERROR|LS::PARSE) << "Parser: error: precedence must be > 1\n";
      prec = 2;
    }
    TokenProto p(Token(TOKEN_IDENTIFIER, s), prec, MODE_POSTFIX);
    ls(LS::CONFIG|LS::INFO) << "Adding postfix named operator: " << p << "\n";
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addPostfix(ParserToken t, int prec) {
    if (prec <= 1) {
      ls(LS::ERROR|LS::PARSE) << "Parser: error: precedence must be > 1\n";
      prec = 2;
    }
    TokenProto p(Token(t, "unknown"), prec, MODE_POSTFIX);
    ls(LS::CONFIG|LS::INFO) << "Adding postfix operator: " << p << "\n";
    if (t == TOKEN_JUXTA) {
      ls(LS::ERROR|LS::PARSE) << "error: Parser: cannot classify JUXTA as Postfix\n";
    }
    prototypes[t] = p;
    return true;
  }

  bool addIgnore(std::string const &s) {
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_IGNORE);
    ls(LS::CONFIG|LS::INFO) << "Adding named ignored item: " << p << "\n";
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addIgnore(ParserToken t) {
    TokenProto p(Token(t, "unknown"), 0, MODE_IGNORE);
    ls(LS::CONFIG|LS::INFO) << "Adding ignored item: " << p << "\n";
    if (t == TOKEN_JUXTA) {
      ls(LS::ERROR|LS::PARSE) << "error: Parser: cannot classify JUXTA as Ignore\n";
    }
    prototypes[t] = p;
    return true;
  }

  bool addItem(std::string const &s) {
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_ITEM);
    ls(LS::CONFIG|LS::INFO) << "Adding named item: " << p << "\n";
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addItem(ParserToken t) {
    TokenProto p(Token(t, "unknown"), 0, MODE_ITEM);
    ls(LS::CONFIG|LS::INFO) << "Adding item: " << p << "\n";
    if (t == TOKEN_JUXTA) {
      ls(LS::ERROR|LS::PARSE) << "error: Parser: cannot classify JUXTA as Item\n";
    }
    prototypes[t] = p;
    return true;
  }

  bool addRBrace(TokenProto &tp) {
    tp.isRParen = 1;
    tp.mode = MODE_PAREN;
    unsigned code = mkOpCode(tp);
    OpPrototypes::iterator it = opPrototypes.find(code);
    if (it != opPrototypes.end()) {
      ls(LS::CONFIG|LS::ERROR) << "Overriding declaration of rbrace " << it->second << " with " << tp
                               << " with mode " << MODE_PAREN << "\n";
      exit(EXIT_FAILURE);
    } else {
      opPrototypes[code] = tp;
    }
    return true;
  }

  void addRBraces(TokenProto &tp) {
    for (auto it = tp.endList.begin(); it != tp.endList.end(); ++it) {
      addRBrace(it->second);
    }
  }

  bool addBrace(std::string const &name, TokenProto &tp) {
    tp.isParen = 1;
    tp.mode = MODE_ITEM;
    unsigned code = mkOpCode(name);
    OpPrototypes::iterator it = opPrototypes.find(code);
    if (it != opPrototypes.end()) {
      if (not it->second.isParen) {
        ls(LS::CONFIG|LS::ERROR) << "Overriding declaration of " << name << " as mode " << it->second.mode
                                 << " with mode " << MODE_PAREN << "\n";
        exit(EXIT_FAILURE);
        //        opPrototypes[code] = tp;
      } else {
        it->second.endList.insert(tp.endList.begin(), tp.endList.end());
        addRBraces(tp);
      }
    } else {
      opPrototypes[code] = tp;
      addRBraces(tp);
    }
    return true;
  }
  bool addBrace(ParserToken t, TokenProto &tp) {
    tp.isParen = 1;
    tp.mode = MODE_ITEM;
    Prototypes::iterator it = prototypes.find(t);
    if (it != prototypes.end()) {
      if (not it->second.isParen) {
        ls(LS::CONFIG|LS::ERROR) << "Overriding declaration of " << t << " as mode " << it->second.mode
                                   << " with mode " << MODE_PAREN << "\n";
        exit(EXIT_FAILURE);
        //        prototypes[t] = tp;
      } else {
        it->second.endList.insert(tp.endList.begin(), tp.endList.end());
        addRBraces(tp);
      }
    } else {
      prototypes[t] = tp;
      addRBraces(tp);
    }
    return true;
  }

  bool addBrace(std::string const &s, std::string const &send) {
    EndList el;
    el.insert(std::make_pair(mkOpCode(send), TokenProto(Token(TOKEN_IDENTIFIER, send))));
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_PAREN, ASSOC_NONE, el);
    ls(LS::CONFIG|LS::INFO) << "Adding named brace: " << p << "\n";
    addBrace(s, p);
    return true;
  }
  bool addBrace(std::string const &s, ParserToken tend) {
    EndList el;
    el.insert(std::make_pair(tend, TokenProto(Token(tend, ""))));
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_PAREN, ASSOC_NONE, el);
    ls(LS::CONFIG|LS::INFO) << "Adding named brace: " << p << "\n";
    addBrace(s, p);
    return true;
  }
  bool addBrace(ParserToken t, std::string const &send) {
    EndList el;
    el.insert(std::make_pair(mkOpCode(send), TokenProto(Token(TOKEN_IDENTIFIER, send))));
    TokenProto p(Token(t, ""), 0, MODE_PAREN, ASSOC_NONE, el);
    ls(LS::CONFIG|LS::INFO) << "Adding named brace: " << p << "\n";
    addBrace(t, p);
    return true;
  }
  bool addBrace(ParserToken t, ParserToken tend) {
    EndList el;
    el.insert(std::make_pair(tend, TokenProto(Token(tend, ""))));
    TokenProto p(Token(t, ""), 0, MODE_PAREN, ASSOC_NONE, el);
    ls(LS::CONFIG|LS::INFO) << "Adding named brace: " << p << "\n";
    addBrace(t, p);
    return true;
  }

  bool isNamedType(Token const * const t) const {
    if (t->token == TOKEN_IDENTIFIER) {
      OpPrototypes::const_iterator it = opPrototypes.find(getOpCode(t));
      return it != opPrototypes.end();
    }
    return false;
  }

  static bool isOp(ParserMode mode) { 
    return (mode == MODE_BINARY
            or mode == MODE_UNARY_BINARY
            or mode == MODE_UNARY
            or mode == MODE_POSTFIX);
  }

  bool isOp(Token *t) const { 
    return isOp(this->mode(t));
  }

  TokenProto const *getProto(Token const * const t) const {
    TokenProto const *res = 0;
    Prototypes::const_iterator it = prototypes.find(t->token);
    if (it != prototypes.end()) {
      res = &it->second;
    }
    if (t->token == TOKEN_IDENTIFIER) {
      OpPrototypes::const_iterator it = opPrototypes.find(getOpCode(t));
      if (it != opPrototypes.end()) {
        res = &it->second;
      }
    }
    if (((res and res->isRParen) or res == 0) and t->left) {
      return getProto(t->left);
    }
    return res;
  }
  
  TokenProto *getProto(Token const * const t) {
    return const_cast<TokenProto*>(const_cast<TokenInfo const *>(this)->getProto(t));
  }
  
  /*
  void setProto(TokenProto const &t) {
    TokenProto const *exProto = getProto(&t);

    if (t.token == TOKEN_IDENTIFIER) {
      OpPrototypes::const_iterator it = opPrototypes.find(getOpCode(&t));
      if (it != opPrototypes.end()) {
        ls(LS::CONFIG|LS::ERROR) << "Token prototype for " << t.token << " already exists" << "\n";
        exit(EXIT_FAILURE);
      }
      opPrototypes[getOpCode(&t)] = t;
    } else {
      Prototypes::const_iterator it = prototypes.find(t.token);
      if (it != prototypes.end()) {
        ls(LS::CONFIG|LS::ERROR) << "Token prototype for " << t.token << " already exists" << "\n";
        exit(EXIT_FAILURE);
      }
      prototypes[t.token] = t;
    }
  }
  */
  
  int prec(Token const * const t) const {
    int res = INT_MAX;
    ParserMode m = mode(t);
    if (isOp(m)) {
      res = 1;
      if (t->token == TOKEN_ROOT) {
        res = 0;
      } else {
        TokenProto const *proto = getProto(t);
        if (proto) {
          // attention: this is only valid for a node that is already
          // in the tree! for the node to be inserted by pushBinary,
          // use method binary_prec below!
          if (m == MODE_UNARY_BINARY and t->left == 0) {
            res = proto->unaryPrecedence;
          } else {
            res = proto->precedence;
          }
        } 
      }
    }
    return res;
  }
  int binary_prec(Token const * const t) const {
    int res = std::numeric_limits<int>::max();
    ParserMode m = mode(t);
    if (isOp(m)) {
      res = 1;
      if (t->token == TOKEN_ROOT) {
        res = 0;
      } else {
        TokenProto const *proto = getProto(t);
        if (proto) {
          res = proto->precedence;
        }
      }
    }
    return res;
  }
  int unary_prec(Token const * const t) const {
    int res = std::numeric_limits<int>::max();
    ParserMode const m = mode(t);
    assert(m == MODE_UNARY || m == MODE_UNARY_BINARY);
    if (isOp(m)) {
      res = 1;
      if (t->token == TOKEN_ROOT) {
        res = 0;
      } else {
        TokenProto const *proto = getProto(t);
        if (proto) {
          if (m == MODE_UNARY) {
            res = proto->precedence;
          } else if (m == MODE_UNARY_BINARY) {
            res = proto->unaryPrecedence;
          }
        }
      }
    }
    return res;
  }
  OutputMode outputMode(Token const * const t) const {
    OutputMode res = OUTPUT_MODE_NONE;
    ParserMode m = mode(t);
    if (m == MODE_BINARY or m == MODE_UNARY_BINARY) {
      if (t->token == TOKEN_ROOT) {
      } else {
        TokenProto const *proto = getProto(t);
        if (proto) {
          res = proto->outputMode;
        }
      }
    }
    return res;
  }
  ParserAssoc assoc(Token const * const t) const {
    ParserAssoc res = ASSOC_NONE;
    ParserMode m = mode(t);
    if (m == MODE_BINARY or m == MODE_UNARY_BINARY) {
      if (t->token == TOKEN_ROOT) {
      } else {
        TokenProto const *proto = getProto(t);
        if (proto) {
          res = proto->associativity;
        }
      }
    }
    return res;
  }
  ParserMode mode(Token const * const t) const {
    ParserMode res = t->token == TOKEN_ROOT ? MODE_BINARY : MODE_ITEM;
    TokenProto const *proto = getProto(t);
    if (proto) {
      res = proto->mode;
    }
    return res;
  }
  bool isParen(Token const * const t) const {
    bool res = false;
    TokenProto const *proto = getProto(t);
    if (proto) {
      res = proto->isParen || proto->isRParen;
    }
    return res;
  }
  EndList endList(Token const * const t) const {
    EndList res;
    TokenProto const *proto = getProto(t);
    if (proto) {
      res = proto->endList;
    }
    return res;
  }

};


struct Lexer {
  ScannerType m_type;
  FlexLexer *m_flexLexer;
  std::istringstream m_istrstr;

  Lexer(ScannerType type = SCANNER_STRINGS) : 
    m_type(type), m_flexLexer() 
  {
    ls(LS::CONFIG)  << "Creating scanner of type " << getScannerTypeName(m_type) << "\n";
    switch(type) {
    case SCANNER_STRINGS:
      m_flexLexer = new scanFlexLexer();
      break;
    case SCANNER_NO_STRINGS:
      m_flexLexer = new scanNSFlexLexer();
      break;
    case SCANNER_R:
      m_flexLexer = new scanRFlexLexer();
      break;
    case SCANNER_C:
      m_flexLexer = new scanCFlexLexer();
      break;
    default:
      break;
    }
  }

  ~Lexer() {
    if (m_flexLexer) {
      delete m_flexLexer;
      m_flexLexer = 0;
    }
  }

  void setStream(std::istream &ins) {
    m_flexLexer->switch_streams(&ins, &std::cout);
  }
  void setString(std::string const &data) {
    m_istrstr.clear();
    m_istrstr.str(data);
    setStream(m_istrstr);
  }
  ParserToken yylex() {
    return ParserToken(m_flexLexer->yylex());
  }
  std::string text() {
    return m_flexLexer->YYText();
  }
};

struct TokenTypeEqual {

  TokenInfo const &tokenInfo;

  TokenTypeEqual(TokenInfo const &tokenInfo) : tokenInfo(tokenInfo) {}

  bool operator()(Token const *s, Token const *t) const { 
    return s->token == t->token
      and (not (tokenInfo.isNamedType(s) or tokenInfo.isNamedType(t)) or s->text == t->text);
  }

};


struct Scanner {

  ScannerType m_type;
  std::vector<Token*> tokenList;

  Scanner(ScannerType type = SCANNER_STRINGS) : 
    m_type(type)
  {}

  ~Scanner() {
    clear();
  }

  void clear() {
    for (size_t i = 0; i < tokenList.size(); ++i) {
      delete tokenList[i];
    }
    tokenList.clear();
  }

  void readTokenList(std::istream &ins) {
    Lexer flexLexer(m_type);
    flexLexer.setStream(ins);
    ParserToken tok = TOKEN_EOF;
    unsigned col = 0, count = 0, line = 1;
    do {
      tok = ParserToken(flexLexer.yylex());
      std::string yytext(flexLexer.text());
      Token *n = new Token(tok, yytext, line, col, count);
      ls(LS::DEBUG|LS::SCAN) << "token: " << tok << ": " << *n << "\n";
      tokenList.push_back(n);
      count += yytext.size();
      if (tok == TOKEN_NEWLINE) {
        col = 0;
        ++line;
      } else {
        col += yytext.size();
      }
    } while (tok != TOKEN_EOF);
  }
};

struct Parser {
  struct Options {
    bool ignoreIgnore;
    Options() {
      ignoreIgnore = false;
    }
  };
  Token *root;
  TokenInfo const &tokenInfo;
  Options const &options;
  TokenList &tokenList;
  EndList endList;
  typedef std::map<int, Token*> LPrecMap;
  LPrecMap leastMap;

  Parser(TokenInfo const &tokenInfo, Options const &options, TokenList &tokenList) :
    root(), tokenInfo(tokenInfo), options(options), tokenList(tokenList) {
    endList.insert(std::make_pair(unsigned(TOKEN_EOF), Token(TOKEN_EOF, "")));
  }

  ~Parser() {
    // delete root;
    root = 0;
  }

  Token *getRM() const {
    LPrecMap::const_iterator it = leastMap.end();
    --it;
    return it->second;
  }
  static bool isOp(ParserMode mode) { 
    return TokenInfo::isOp(mode);
  }
  bool isOp(Token *t) const { 
    return tokenInfo.isOp(t);
  }
  Token *getRMOp() const {
    LPrecMap::const_iterator it = leastMap.end();
    --it;
    if (tokenInfo.mode(it->second) == MODE_ITEM)
      --it;
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "rmop = " << it->second << " " << *it->second << " " << tokenInfo.prec(it->second) << "\n";
#endif
    assert(tokenInfo.mode(it->second) != MODE_ITEM);
    return it->second;
  }
  Token *old_getRMOp(Token *t) const {
    while (t->right and isOp(t->right)) {
      t = t->right;
    }
    return t;
  }
  bool rightEdgeOpen() const {
    Token *rm = getRMOp();
    return rm->right == 0 and tokenInfo.mode(rm) != MODE_POSTFIX;
  }

  bool tokenTypeEqual(Token const *s, Token const *t) const { 
    return TokenTypeEqual(tokenInfo)(s, t);
    // return s->token == t->token
    //   and (not (tokenInfo.isNamedType(s) or tokenInfo.isNamedType(t)) or s->text == t->text);
  }

  Token *mkRoot() {
    // return new Token(TOKEN_ROOT, "");
    tokenList.tokenList.push_back(new Token(TOKEN_ROOT, ""));
    return tokenList.tokenList.back();
  }
  Token *mkJuxta(Token const * const t, ParserToken token = TOKEN_JUXTA) {
    // return new Token(TOKEN_JUXTA, "", t->line, t->column, t->character);
    tokenList.tokenList.push_back(new Token(token, "", t->line, t->column, t->character));
    return tokenList.tokenList.back();
  }

  // The push methods
  // Class Item
  void pushItem_(Token *t) {
    assert(root);
    Token *rmop = getRMOp();
    if (tokenInfo.mode(rmop) == MODE_POSTFIX or rmop->right != 0) {
      Token *op = mkJuxta(t);
      pushBinary(op);
#ifndef NDEBUG
      Token *rmop2 = getRMOp();
#endif
      assert(rmop2->right == 0);
      assert(rmop2 == op);
      op->right = t;
    } else {
      rmop->right = t;
    }
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "rmop prec = " << tokenInfo.prec(rmop) << "\n";
#endif
  }
  void pushItem(Token *t) {
    pushItem_(t);
#ifndef NDEBUG
    int const prec = INT_MAX; // == tokenInfo.prec(t);
    ls(LS::DEBUG|LS::PARSE) << "prec = " << prec << "\n";
#endif
    leastMap[INT_MAX] = t;
  }

  // Class Unary
  void pushUnary(Token *t) {
    pushItem_(t);
    int const prec = tokenInfo.unary_prec(t);
    leastMap[prec] = t;
    leastMap.erase((++leastMap.find(prec)), leastMap.end());
  }

  // Class Binary
  void pushBinary(Token *t) {
    assert(root);
    Token *tmp = 0, *parent = 0;
    ParserAssoc const assoc = tokenInfo.assoc(t);
    int const prec = tokenInfo.binary_prec(t);

#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "prec = " << prec << "\n";
    ls(LS::DEBUG|LS::PARSE) << "Looking for " << prec << " in the map" << "\n";
    ls(LS::DEBUG|LS::PARSE) << " assoc " << assoc << "" << "\n";
    {
      LPrecMap::iterator it = leastMap.begin();
      for (; it != leastMap.end(); ++it) {
        ls(LS::DEBUG|LS::PARSE) << "Item: " << it->first << " " << it->second << "" << "\n";
      }
    }
#endif

    LPrecMap::iterator it = leastMap.lower_bound(prec + (assoc == ASSOC_RIGHT ? 1 : 0));

    --it;
    while(tokenInfo.mode(it->second) == MODE_POSTFIX) --it;

    parent = it->second;
    tmp = parent->right; // !!!

    parent->right = t;
    if (tmp) {
      if (tokenInfo.isParen(t) and t->left) {
        assert(t->left->left == 0);
        t->left->left = tmp;
      } else {
        assert(t->left == 0);
        t->left = tmp;
      }
    }

    leastMap[prec] = t;
    leastMap.erase((++leastMap.find(prec)), leastMap.end());
  }

  // Class Unary/Binary
  void pushUnaryBinary(Token *t) {
    if (rightEdgeOpen()) {
      pushUnary(t);
    } else {
      pushBinary(t);
    }
  }

  // Class Postfix
  void pushPostfix(Token *t) {
    pushBinary(t);
  }

  // Class Ignore
  void pushIgnore(Token *t) {
    if (not options.ignoreIgnore) {
      Token *rm = getRM();
      Token *tend = t;
      while (tend->ignore) 
        tend = tend->ignore;
      tend->ignore = rm->ignore;
      rm->ignore = t;
    }
  }

  // here we have to look "inside" the parens to find the ignore
  // insert position. this is the old behaviour
  void pushIgnoreAsBefore(Token *t) {
    if (not options.ignoreIgnore) {
      Token *rm = getRM();
      t->ignore = rm->ignore;
      rm->ignore = t;
    }
  }

  void insertToken(Token *first) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "insert Token: " << (void*)first << " " << *first << "\n";
#endif
    ParserMode firstMode = tokenInfo.mode(first);
    assert(not(firstMode & MODE_PAREN)); // MODE_PAREN bit is cleared
    assert(firstMode != 0); // mode is not 0
    assert(firstMode != MODE_PAREN); // mode is not MODE_PAREN
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "mode = " << firstMode << "\n";
#endif
    switch(firstMode) {
    case MODE_ITEM:
      pushItem(first);
      break;
    case MODE_IGNORE:
      pushIgnoreAsBefore(first);
      break;
    case MODE_UNARY:
      pushUnary(first);
      break;
    case MODE_BINARY:
      pushBinary(first);
      break;
    case MODE_POSTFIX:
      pushPostfix(first);
      break;
    case MODE_UNARY_BINARY:
      pushUnaryBinary(first);
      break;
    default:
      ls(LS::ERROR|LS::PARSE) << "Parser: error: invalid mode " << firstMode << "\n";
      exit(1);
      break;
    }
  }

  Token *parse() {
    Token *first = 0;
    root = mkRoot();
    leastMap[tokenInfo.prec(root)] = root;
    bool endFound = false;
    do {
      // first = new Token(tokenList.next());
      first = tokenList.next();
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "Parser: next: " << *first
           << ": mode: " << tokenInfo.mode(first)
           << ": prec: " << tokenInfo.prec(first)
           << "\n";
#endif
      if (endList.find(tokenInfo.getOpCode(first)) != endList.end()) {
        endFound = true;
      } else if (first->token == TOKEN_EOF) {
        ls(LS::WARNING) << "unexpected end of input encountered" << std::endl;
        endFound = true;
      } else if (tokenInfo.isParen(first)) {
        Parser parser(tokenInfo, options, tokenList);
        parser.endList = tokenInfo.endList(first);
        Token *last = parser.parse();


        first->right = parser.root->right;
        if (last->token == TOKEN_EOF) {
          endFound = true;
          insertToken(first);
        } else {
          last->left = first;
          insertToken(last);
        }

        if (parser.root->ignore) {
          // pushIgnore(parser.root->ignore);
          assert(first->ignore == 0);
          first->ignore = parser.root->ignore;
        }

        assert(last->right == 0);

        first = last;

      } else {
        insertToken(first);
      } 
    } while(not endFound);

    if (root->right)
      root->right->rmt = getRM();
    return first;
  }

};

struct FPParser {

  TokenInfo tokenInfo;
  Parser::Options options;
  Scanner scanner;

  FPParser(ScannerType scannerType): 
    scanner(scannerType)
  { }

  ~FPParser() { }

  void clear() {
    scanner.clear();
  }

  Token *parseStream(std::istream &ins) {
    Timer tAll;
    Timer tScan;
    std::ostream &lout = (ls(LS::TIMES) << "Scanning input...");
    scanner.readTokenList(ins);
    lout << "done in " << tScan << " s" << std::endl;
    TokenList tokenList(scanner.tokenList);
    Parser parser(tokenInfo, options, tokenList);
    {
      Timer tParse;
      std::ostream &lout2 = (ls(LS::TIMES) << "Parsing input... ");
      parser.parse();
      lout2 << "done in " << tParse << " s" << std::endl;
      ls(LS::TIMES) << "loaded in " << tAll << " s" << std::endl;
    }
    return parser.root;
  }

};

#define p2x_stack_use_list
// #define p2x_stack_use_vector

#ifdef p2x_stack_use_vector
// use std::vector as stack
#define stackCont vector
#define sPush push_back
#define sPop pop_back
#define sTop back
#elifdef p2x_stack_use_list
// use std::vector as stack
#define stackCont list
#define sPush push_back
#define sPop pop_back
#define sTop back
#else
// use std::stack as stack
#define stackCont stack
#define sPush push
#define sPop pop
#define sTop top
#endif

template <class HandlerClass>
struct TreeTraverser {
  enum State { ST_ENTER, ST_BETWEEN, ST_LEAVE };
  typedef std::pair<State, Token const*> StackItem;
  HandlerClass *m_obj;
  std::stackCont<StackItem> m_stack;
  std::string m_indent;

  void (HandlerClass::*enterFcn)(Token const *, Token const *);
  void (HandlerClass::*leaveFcn)(Token const *, Token const *);
  void (HandlerClass::*contentFcn)(Token const *, Token const *);

  TreeTraverser(HandlerClass *obj) :
    m_obj(obj),
    enterFcn(),
    leaveFcn(),
    contentFcn()
  {}

  void handleNode(StackItem const t) {

    switch (t.first) {
    case ST_ENTER:
      if (enterFcn) {
        (m_obj->*enterFcn)(t.second, m_stack.sTop().second);
      }
      m_stack.sPush(std::make_pair(ST_BETWEEN, t.second));
      if (t.second->left) {
        handleNode(std::make_pair(ST_ENTER, t.second->left));
      }
      break;
    case ST_BETWEEN:
      if (contentFcn) {
        (m_obj->*contentFcn)(t.second, m_stack.sTop().second);
      }
      m_stack.sPush(std::make_pair(ST_LEAVE, t.second));
      if (t.second->right) {
        handleNode(std::make_pair(ST_ENTER, t.second->right));
      }
      break;
    case ST_LEAVE:
      if (leaveFcn) {
        (m_obj->*leaveFcn)(t.second, m_stack.sTop().second);
      }
      break;
    }

  }

  void traverseTree(Token const *t) {
    m_stack.sPush(std::make_pair(ST_ENTER, (Token const*)0));
    m_stack.sPush(std::make_pair(ST_ENTER, t));
    while(m_stack.top().second) {
      StackItem n = m_stack.sTop();
      m_stack.sPop();
      handleNode(n);
    }

  }

};

struct TreeXMLWriter {
  struct Options {
    Options() :
      id(true), line(), col(), _char(),
      prec(), mode(), type(true),
      indent(true), indentLogarithmic(true), newlineAsBr(true),
      newlineAsEntity(false),
      merged(),
      strict(),
      writeRec(true),
      minStraightIndentLevel(135),
      encoding("default is in .ggo")
    {}
    bool id, line, col, _char, prec, mode, type, indent, indentLogarithmic, newlineAsBr, newlineAsEntity, merged, strict, writeRec;
    unsigned minStraightIndentLevel;
    std::string encoding;
  };
  TokenInfo const &tokenInfo;
  Options options;
  std::string indentUnit;
  std::string linebreak;

  TreeXMLWriter (TokenInfo const &tokenInfo,
                 Options const &options,
                 std::string const &_indentUnit = " ",
                 std::string const &_linebreak = "\n") :
    tokenInfo(tokenInfo),
    options(options)
  {
    if (options.indent) {
      indentUnit = _indentUnit;
      linebreak = _linebreak;
    }
  }

  void writeXMLLocAttrs(Token const *t, std::ostream &aus) const {
    if (options.line)
      aus << " line='" << t->line << "'";
    if (options.col)
      aus << " col='" << t->column << "'";
    if (options._char)
      aus << " char='" << t->character << "'";
  }

  void writeXMLTypeElem(Token const *t, std::ostream &aus, std::string const &indent = "") const {
    aus << indent << indentUnit
        << "<ca:type id='" << int(t->token) << "' name='" << Token::getParserTokenName(t->token) << "'/>" << linebreak;
  }

  void writeXMLTypeAttrs(Token const *t, std::ostream &aus) const {
    if (t->token == TOKEN_IDENTIFIER) {
      aus << " code='" << tokenInfo.getOpCode(t) << "'";
      aus << " repr='" << t->text << "'";
    } else {
      aus << " code='" << int(t->token) << "'";
    }
    if (options.type)
      aus << " type='" << Token::getParserTokenName(t->token) << "'";
  }

  void writeXMLPrecAttrs(Token const *t, std::ostream &aus) const {
    if (options.prec)
      aus << " precedence='" << tokenInfo.prec(t) << "'";
    if (options.mode)
      aus << " mode='" << tokenInfo.mode(t) << "'";
  }

  bool writeXMLTextElem(Token const *t, std::ostream &aus, std::string const &indent = "") const {
    bool res = 0;
    if (t->token == TOKEN_NEWLINE) {
      if (options.newlineAsBr and not options.newlineAsEntity) {
        aus << indent << "<ca:br/>";
      } else {
        aus << indent << "<ca:text>";
        if (options.newlineAsEntity) {
          aus << "&#xa;";
        } else {
          aus << t->text;
        }
        aus << "</ca:text>";
      }
      res = 1;
    } else if (t->token == TOKEN_CRETURN) {
      if (options.newlineAsBr) {
        aus << indent << "<ca:cr/>";
      } else {
        aus << indent << "<ca:text>";
        if (options.newlineAsEntity) {
          aus << "&#xd;";
        } else {
          aus << t->text;
        }
        aus << "</ca:text>";
      }
      res = 1;
    } else if (t->text.size()) {
      aus << indent << "<ca:text>";
      XMLOstream x(aus);
      x << t->text;
      aus << "</ca:text>";
      res = 1;
    }
    return res;
  }

  struct TreePrintHelper {
    TreeXMLWriter const &m_xmlWriter;
    size_t m_level;
    std::string indent, subindent, elemName;
    bool merged, tags;
    std::ostream &aus;

    TreePrintHelper(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
      m_xmlWriter(xmlWriter),
      m_level(),
      aus(aus)
    {}

    void setWhiteLen(std::string &str, size_t ilevel) const {
      if (str.size() < ilevel) {
        str.insert(str.end(), ilevel-str.size(), m_xmlWriter.indentUnit[0]);
      }
      if (str.size() > ilevel) {
        str.resize(ilevel);
      }
    }

    void setIndent() {
      if (m_xmlWriter.options.indent) {
        int ilevel = std::min<int>(m_level, m_xmlWriter.options.minStraightIndentLevel + log(std::max<size_t>(m_level, 1)));
#ifndef NDEBUG
        ls(LS::DEBUG|LS::PARSE) << "rec. level -> indent level: " << m_level << " -> " << ilevel << "\n";
#endif
        setWhiteLen(indent, ilevel);
        setWhiteLen(subindent, ilevel+1);
      }
    }

    void setElemName(Token const *t) {
      if (m_xmlWriter.tokenInfo.isParen(t)) {
        elemName = "paren";
      } else if (t->token == TOKEN_STRING) {
        elemName = "string";
      } else if (t->token == TOKEN_FLOAT) {
        elemName = "float";
      } else if (t->token == TOKEN_INTEGER) {
        elemName = "integer";
      } else if (t->token == TOKEN_IDENTIFIER) {
        if (m_xmlWriter.tokenInfo.mode(t) == MODE_ITEM) {
          elemName = "id";
        } else {
          elemName = "op";
        }
      } else if (t->token == TOKEN_ROOT) {
        elemName = "root";
      } else {
        elemName = "op";
      }
    }

    void setupNode(Token const *t) {
      merged = m_xmlWriter.options.merged
        or m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;
    }

    void onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onEnter " << (void*)t << " " << *t << "\n";
#endif
      setupNode(t);
      setElemName(t);
      setIndent();

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged);
      if (tags) {
        aus << indent << "<" << elemName << "";
        if (m_xmlWriter.options.id)
          aus << " id='" << t->id << "'";
        m_xmlWriter.writeXMLLocAttrs(t, aus);
        m_xmlWriter.writeXMLTypeAttrs(t, aus);
        m_xmlWriter.writeXMLPrecAttrs(t, aus);
        aus << ">" << m_xmlWriter.linebreak;
        ++m_level;
      }
      if (t->left == 0 and t->right != 0) {
        aus << indent << m_xmlWriter.indentUnit << "<null/>" << m_xmlWriter.linebreak;
      }

    }
    void onContent(Token const *t, Token const * /* parent */) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif

      setupNode(t);
      setIndent();

      bool const wrt = m_xmlWriter.writeXMLTextElem(t, aus, indent);
      if (wrt) aus << m_xmlWriter.linebreak;
      if (t->ignore) {
        m_xmlWriter.writeIgnoreXML(t->ignore, aus, indent);
      }
    }
    void onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

      setupNode(t);
      setElemName(t);

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged);
      if (tags) {
        --m_level;
        setIndent();
        aus << indent << "</" << elemName << ">" << m_xmlWriter.linebreak;
      }
    }
  };

  struct XMLTreePrintHelper2 {
    TreeXMLWriter const &m_xmlWriter;
    size_t m_level;
    std::string indent, subindent, elemName;
    bool merged, tags;
    std::ostream &aus;
    XMLOstream xaus;

    XMLTreePrintHelper2(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
      m_xmlWriter(xmlWriter),
      m_level(),
      aus(aus),
      xaus(aus)
    {}

    void setWhiteLen(std::string &str, size_t ilevel) const {
      if (str.size() < ilevel) {
        str.insert(str.end(), ilevel-str.size(), m_xmlWriter.indentUnit[0]);
      }
      if (str.size() > ilevel) {
        str.resize(ilevel);
      }
    }

    void setIndent() {
      if (m_xmlWriter.options.indent) {
        int ilevel = std::min<int>(m_level, m_xmlWriter.options.minStraightIndentLevel + log(std::max<size_t>(m_level, 1)));
#ifndef NDEBUG
        ls(LS::DEBUG|LS::PARSE) << "rec. level -> indent level: " << m_level << " -> " << ilevel << "\n";
#endif
        setWhiteLen(indent, ilevel);
        setWhiteLen(subindent, ilevel+1);
      }
    }

  void writeIgnoreXML(Token *t, std::string const &indent = "") {
    if (t->ignore) {
      writeIgnoreXML(t->ignore, indent);
    }
    aus << indent << "<ci:" << Token::getParserTokenName(t->token);
    if (m_xmlWriter.options.id)
      aus << " id='" << t->id << "'";
    m_xmlWriter.writeXMLLocAttrs(t, aus);
    // writeXMLTypeAttrs(t, aus);
    aus << ">" << t->text << "</ci:" << Token::getParserTokenName(t->token) << ">" << m_xmlWriter.linebreak;
  }

  bool writeXMLTextElem(Token const *t, std::string const &indent = "") {
    bool res = 0;
    if (t->text.size() and (t->left or t->right or t->ignore)) {
      aus << indent;
    }
    if (t->token == TOKEN_NEWLINE) {
      if (m_xmlWriter.options.newlineAsBr) {
        aus << "<c:br/>";
      } else {
        aus << "<c:t>";
        if (m_xmlWriter.options.newlineAsEntity) {
          aus << "&#xa;";
        } else {
          aus << t->text;
        }
        aus << "</c:t>";
      }
      res = 1;
    } else if (t->token == TOKEN_CRETURN) {
      if (m_xmlWriter.options.newlineAsBr) {
        aus << "<c:cr/>";
      } else {
        aus << "<c:t>";
        if (m_xmlWriter.options.newlineAsEntity) {
          aus << "&#xd;";
        } else {
          aus << t->text;
        }
        aus << "</c:t>";
      }
      res = 1;
    } else if (t->text.size()) {
      aus << "<c:t>";
      xaus << t->text;
      aus << "</c:t>";
      res = 1;
    }
    return res;
  }
    void setElemName(Token const *t) {
      if (t->token == TOKEN_IDENTIFIER) {
        if (t->left || t->right) {
          elemName = t->text;
        } else {
          elemName = "ID";
        }
      } else if (t->token == TOKEN_INTEGER)
        elemName = "INT";
      else if (t->token == TOKEN_FLOAT)
        elemName = "DBL";
      else
        elemName = Token::getParserTokenName(t->token);
    }

    void setupNode(Token const *t) {
      merged = m_xmlWriter.options.merged
        or m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;
    }

    void onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onEnter " << (void*)t << " " << *t << "\n";
#endif
      setupNode(t);
      setElemName(t);
      setIndent();

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged);
      if (tags) {
        aus << indent << "<" << elemName << "";
        if (m_xmlWriter.options.id)
          aus << " id='" << t->id << "'";
        m_xmlWriter.writeXMLLocAttrs(t, aus);
        aus << ">";
        if (t->left or t->right or t->ignore) {
          aus << m_xmlWriter.linebreak;
          ++m_level;
        }
      }
      if (t->left == 0 and t->right != 0) {
        aus << indent << m_xmlWriter.indentUnit << "<null/>" << m_xmlWriter.linebreak;
      }

    }
    void onContent(Token const *t, Token const * /* parent */) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif

      setupNode(t);
      setIndent();

      bool const wrt = writeXMLTextElem(t, indent);
      if (wrt and (t->left or t->right or t->ignore)) aus << m_xmlWriter.linebreak;
      if (t->ignore) {
        writeIgnoreXML(t->ignore, indent);
      }
    }
    void onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

      setupNode(t);
      setElemName(t);

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged);
      if (tags) {
        if (t->left or t->right or t->ignore) {
          --m_level;
          setIndent();
          aus << indent;
        }
        aus << "</" << elemName << ">" << m_xmlWriter.linebreak;
      }
    }
  };

  #include "TreePrintHelperMatlabLR.hh"
  #include "TreePrintHelperMatlabChildren.hh"

  struct TreePrintHelperJSON {
    TreeXMLWriter const &m_xmlWriter;
    size_t m_level;
    std::string indent, subindent, elemName;
    bool merged, tags;
    std::ostream &aus;

    TreePrintHelperJSON(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
      m_xmlWriter(xmlWriter),
      m_level(),
      aus(aus)
    {}

    void setWhiteLen(std::string &str, size_t ilevel) const {
      if (str.size() < ilevel) {
        str.insert(str.end(), ilevel-str.size(), m_xmlWriter.indentUnit[0]);
      }
      if (str.size() > ilevel) {
        str.resize(ilevel);
      }
    }

    void setIndent() {
      if (m_xmlWriter.options.indent) {
        int ilevel = std::min<int>(m_level, m_xmlWriter.options.minStraightIndentLevel + log(std::max<size_t>(m_level, 1)));
#ifndef NDEBUG
        ls(LS::DEBUG|LS::PARSE) << "rec. level -> indent level: " << m_level << " -> " << ilevel << "\n";
#endif
        setWhiteLen(indent, ilevel);
        setWhiteLen(subindent, ilevel+1);
      }
    }

    void setElemName(Token const *t) {
      if (m_xmlWriter.tokenInfo.isParen(t)) {
        elemName = "paren";
      } else if (t->token == TOKEN_STRING) {
        elemName = "string";
      } else if (t->token == TOKEN_FLOAT) {
        elemName = "float";
      } else if (t->token == TOKEN_INTEGER) {
        elemName = "integer";
      } else if (t->token == TOKEN_IDENTIFIER) {
        if (m_xmlWriter.tokenInfo.mode(t) == MODE_ITEM) {
          elemName = "id";
        } else {
          elemName = "op";
        }
      } else if (t->token == TOKEN_ROOT) {
        elemName = "root";
      } else {
        elemName = "op";
      }
    }

    void setupNode(Token const *t) {
      merged = m_xmlWriter.options.merged
        or m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;
    }

    void writeSFLocAttrs(Token const *t, std::ostream &aus) const {
      if (m_xmlWriter.options.line)
        aus << ",\"line\":" << t->line << "";
      if (m_xmlWriter.options.col)
        aus << ",\"col\":" << t->column << "";
      if (m_xmlWriter.options._char)
        aus << ",\"char\":\"" << t->character << "\"";
    }

    void writeSFTypeAttrs(Token const *t, std::ostream &aus) const {
      if (t->token == TOKEN_IDENTIFIER) {
        aus << ",\"code\":" << m_xmlWriter.tokenInfo.getOpCode(t) << "";
        aus << ",\"repr\":\"" << t->text << "\"";
      } else {
        aus << ",\"code\":" << int(t->token) << "";
      }
      if (m_xmlWriter.options.type)
        aus << ",\"type\":\"" << Token::getParserTokenName(t->token) << "\"";
    }

    void onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onEnter " << (void*)t << " " << *t << "\n";
#endif
      setupNode(t);
      setElemName(t);
      setIndent();

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged) and m_xmlWriter.tokenInfo.mode(t) != MODE_ITEM;
      if (tags) {
        aus << indent << ",{";
        aus << "\"type\":\"" << Token::getParserTokenName(t->token) << "\"";
        if (m_xmlWriter.options.id)
          aus << ",\"id\":" << t->id << "";
        // m_xmlWriter.writeSFLocAttrs(t, aus);
        // m_xmlWriter.writeSFTypeAttrs(t, aus);
        // aus << ")\n";
        aus << ",\"child\":[0";
        ++m_level;
      }
      if (not t->left and m_xmlWriter.options.strict) {
        aus << ",\"\"";
      }

    }
    void onContent(Token const *t, Token const * /* parent */) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif

      setupNode(t);
      setIndent();

      if (t->token == TOKEN_INTEGER || t->token == TOKEN_FLOAT) {
        aus << ",";
        if (t->text.substr(0,1).find_first_of("0123456789") == std::string::npos) {
          aus << "0";
        }
        aus << t->text << "";
      } else if (t->token == TOKEN_NEWLINE) {
        aus << ",\"\\n\"\n";
      } else if (not t->text.empty()) {
        aus << ",\"" << t->text << "\"" << "";
      } else {
        aus << ",\"\"";
      }
    }
    void onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

      setupNode(t);
      setElemName(t);

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged) and m_xmlWriter.tokenInfo.mode(t) != MODE_ITEM;
      if (tags) {
        --m_level;
        setIndent();
        aus << indent << "]}\n";
      }
    }
  };

  void writeXML(Token const *t, std::ostream &aus, 
                std::string const &v = "", Token const *w = 0,
                int level = 0) const {
    if (options.writeRec) {
      writeXML_Rec(t, aus, v, w, level);
    } else {
      writeXML_Stack(t, aus, v, w, level);
    }
  }
  void writeXML2_Stack(Token const *t, std::ostream &aus,
                std::string const & = "", Token const * = 0,
                int level = 0) const {

    XMLTreePrintHelper2 printer(*this, aus);
    printer.m_level = level + 1;

    TreeTraverser<XMLTreePrintHelper2> traverser(&printer);

    traverser.enterFcn = &XMLTreePrintHelper2::onEnter;
    traverser.contentFcn = &XMLTreePrintHelper2::onContent;
    traverser.leaveFcn = &XMLTreePrintHelper2::onLeave;

    traverser.traverseTree(t);

  }
  void writeXML_Stack(Token const *t, std::ostream &aus,
                std::string const & = "", Token const * = 0,
                int level = 0) const {

    TreePrintHelper printer(*this, aus);
    printer.m_level = level + 1;

    TreeTraverser<TreePrintHelper> traverser(&printer);

    traverser.enterFcn = &TreePrintHelper::onEnter;
    traverser.contentFcn = &TreePrintHelper::onContent;
    traverser.leaveFcn = &TreePrintHelper::onLeave;

    traverser.traverseTree(t);

  }
  template<class TreePrintHelperMATLAB>
  void TwriteMATLAB_Stack(Token const *t, std::ostream &aus,
                std::string const & = "", Token const * = 0,
                int level = 0) const {

    TreePrintHelperMATLAB printer(*this, aus);
    printer.m_level = level + 1;

    TreeTraverser<TreePrintHelperMATLAB> traverser(&printer);

    traverser.enterFcn = &TreePrintHelperMATLAB::onEnter;
    traverser.contentFcn = &TreePrintHelperMATLAB::onContent;
    traverser.leaveFcn = &TreePrintHelperMATLAB::onLeave;

    traverser.traverseTree(t);
  }
  void writeMATLAB_Stack(Token const *t, std::ostream &aus,
                std::string const &s = "", Token const *v = 0,
                int level = 0) const {

    TwriteMATLAB_Stack<TreePrintHelperMATLABChildren>(t, aus, s, v, level);
  }
  void writeMATLAB_LR_Stack(Token const *t, std::ostream &aus,
                std::string const &s = "", Token const *v = 0,
                int level = 0) const {

    TwriteMATLAB_Stack<TreePrintHelperMATLABLR>(t, aus, s, v, level);
  }
  void writeJSON_Stack(Token const *t, std::ostream &aus,
                std::string const & = "", Token const * = 0,
                int level = 0) const {

    TreePrintHelperJSON printer(*this, aus);
    printer.m_level = level + 1;

    aus << "{\"code-tree\":0,\"child\":[0";

    TreeTraverser<TreePrintHelperJSON> traverser(&printer);

    traverser.enterFcn = &TreePrintHelperJSON::onEnter;
    traverser.contentFcn = &TreePrintHelperJSON::onContent;
    traverser.leaveFcn = &TreePrintHelperJSON::onLeave;

    traverser.traverseTree(t);
    aus << "]}\n";
  }

  void writeXML_Rec(Token const *t, std::ostream &aus,
                 std::string const &indent_ = "", Token const *parent = 0,
                 int level = 0) const {

    if (level == 0) {
      level = ceil(double(indent_.size())/indentUnit.size());
    }
    std::string indent = indent_;
    std::string elemName;
    if (tokenInfo.isParen(t)) {
      elemName = "paren";
    } else if (t->token == TOKEN_STRING) {
      elemName = "string";
    } else if (t->token == TOKEN_FLOAT) {
      elemName = "float";
    } else if (t->token == TOKEN_INTEGER) {
      elemName = "integer";
    } else if (t->token == TOKEN_IDENTIFIER) {
      if (tokenInfo.mode(t) == MODE_ITEM) {
        elemName = "id";
      } else {
        elemName = "op";
      }
    } else if (t->token == TOKEN_ROOT) {
      elemName = "root";
    } else {
      elemName = "op";
    }

    bool const merged = options.merged 
      or tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;
    bool const tags = not(parent
                          and TokenTypeEqual(tokenInfo)(parent, t)
                          and merged);

    std::string subindent = indent;
    if ( tags and (!options.indentLogarithmic or double(indent.size()) / indentUnit.size() < options.minStraightIndentLevel + log(level) ) ) {
      subindent += indentUnit;
    }
    if (tags) {
      aus << indent << "<" << elemName << "";
      if (options.id)
        aus << " id='" << t->id << "'";
      writeXMLLocAttrs(t, aus);
      writeXMLTypeAttrs(t, aus);
      writeXMLPrecAttrs(t, aus);
      aus << ">" << linebreak;
    }
    if (t->left != 0) {
      Token const *passParent = 0;
      if (merged and tokenInfo.assoc(t) != ASSOC_RIGHT) {
        passParent = t;
      }
      writeXML_Rec(t->left, aus, subindent, passParent, level+1);
    } else if (t->right != 0) {
      aus << indent << indentUnit << "<null/>" << linebreak;
    }
    bool const wrt = writeXMLTextElem(t, aus, subindent);
    if (wrt) aus << linebreak;
    if (t->ignore) {
      writeIgnoreXML(t->ignore, aus, subindent);
    }
    if (t->right != 0) {
      Token const *passParent = 0;
      if (merged and tokenInfo.assoc(t) == ASSOC_RIGHT) {
        passParent = t;
      }
      writeXML_Rec(t->right, aus, subindent, passParent, level+1);
    } 
    if (tags) {
      aus << indent << "</" << elemName << ">" << linebreak;
    }
  }

  void writeIgnoreXML(Token *t, std::ostream &aus, std::string const &indent = "") const {
    if (t->ignore) {
      writeIgnoreXML(t->ignore, aus, indent);
    }
    aus << indent << "<ca:ignore";
    if (options.id)
      aus << " id='" << t->id << "'";
    writeXMLLocAttrs(t, aus);
    writeXMLTypeAttrs(t, aus);
    aus << ">";
    writeXMLTextElem(t, aus);
    aus << "</ca:ignore>" << linebreak;
  }

  void writeXML(EndList const &endList, std::ostream &aus, std::string const &indent = "") const {
    aus << indent << "<ca:closing-list>" << linebreak;
    for (EndList::const_iterator eit = endList.begin(); eit != endList.end(); ++eit) {
      writeXML(eit->second, aus, indent + indentUnit);
    }
    aus << indent << "</ca:closing-list>" << linebreak;
  }

  void writeXML(TokenProto const &tp, std::ostream &aus, std::string const &indent = "") const {
    aus << indent << "<ca:op";
    writeXMLTypeAttrs(&tp, aus);
    aus << " mode='" << tp.mode << "'";
    if (Parser::isOp(tp.mode)) {
      aus << " precedence='" << tp.precedence << "'";
      aus << " associativity='" << tp.associativity << "'";
      if (tp.outputMode)
        aus << " output-mode='" << getOutputModeName(tp.outputMode) << "'";
    }
    if (tp.mode == MODE_UNARY_BINARY) {
      aus << " unary-precedence='" << tp.unaryPrecedence << "'";
    }
    if (tp.isParen) {
      aus << " is-paren='1'";
      aus << ">" << linebreak;
      writeXML(tp.endList, aus, indent + indentUnit);
      aus << indent << "</ca:op>" << linebreak;
    } else {
      aus << "/>" << linebreak;
    }
  }

  void writeXML(TokenInfo const &t, std::ostream &aus, std::string const &indent = "") const {
    aus << indent << "<ca:token-types>" << linebreak;
    {
    TokenInfo::Prototypes::const_iterator it = t.prototypes.begin();
    for(; it != t.prototypes.end(); ++it) {
      writeXML(it->second, aus, indent + indentUnit);
    }}
    {
    TokenInfo::OpPrototypes::const_iterator it = t.opPrototypes.begin();
    for(; it != t.opPrototypes.end(); ++it) {
      writeXML(it->second, aus, indent + indentUnit);
    }}
    aus << indent << "</ca:token-types>" << linebreak;
  }

  void writeXML(TreeXMLWriter const &t, std::ostream &aus, std::string const &indent = "") const {
    aus << indent << "<ca:tree-writer";
    aus << " col='" << t.options.col << "'";
    aus << " merged='" << t.options.merged << "'";
    aus << " encoding='" << t.options.encoding << "'";
    aus << " id='" << t.options.id << "'";
    aus << " indent='" << t.options.indent << "'";
    aus << " line='" << t.options.line << "'";
    aus << " mode='" << t.options.mode << "'";
    aus << " newlineAsBr='" << t.options.newlineAsBr << "'";
    aus << " newlineAsEntity='" << t.options.newlineAsEntity << "'";
    aus << " prec='" << t.options.prec << "'";
    aus << " strict='" << t.options.strict << "'";
    aus << " type='" << t.options.type << "'";
    aus << "/>" << linebreak;
  }

  void writeXML(std::vector<Token *> const &t, std::ostream &aus, std::string const &indent = "") const {
    std::vector<Token *>::const_iterator it = t.begin();
    for(; it != t.end(); ++it) {
      writeXML(*it, aus, indent);
    }
  }

};

void writeTreeXML(Token *root, TokenInfo const &tokenInfo,
                  TreeXMLWriter::Options const &options, std::string const &indentUnit,
                  std::ostream &out, ScannerType scannerType) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
  out << "<code-xml xmlns='" NAMESPACE_CX "' xmlns:ca='" NAMESPACE_CA "'>" << treeXMLWriter.linebreak;
  out << treeXMLWriter.indentUnit << "<ca:steps/>" << treeXMLWriter.linebreak;
  out << treeXMLWriter.indentUnit << "<ca:scanner type='"
      << getScannerTypeName(scannerType) << "'/>" << treeXMLWriter.linebreak;
  treeXMLWriter.writeXML(treeXMLWriter, out, treeXMLWriter.indentUnit);
  treeXMLWriter.writeXML(tokenInfo, out, treeXMLWriter.indentUnit);
  treeXMLWriter.writeXML(root, out, treeXMLWriter.indentUnit);
  out << "</code-xml>\n";
}

void writeTreeXML2(Token *root, TokenInfo const &tokenInfo,
                     TreeXMLWriter::Options const &options, std::string const &indentUnit,
                     std::ostream &out, ScannerType ) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
  out << "<code-xml xmlns='" NAMESPACE_CX "' xmlns:c='" NAMESPACE_CA "' xmlns:ci='" NAMESPACE_CX "ignore' >" << treeXMLWriter.linebreak;
  treeXMLWriter.writeXML2_Stack(root, out, treeXMLWriter.indentUnit);
  out << "</code-xml>\n";
}

void writeTreeMATLAB(Token *root, TokenInfo const &tokenInfo,
                     TreeXMLWriter::Options const &options, std::string const &indentUnit,
                     std::ostream &out, ScannerType ) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  treeXMLWriter.writeMATLAB_Stack(root, out, treeXMLWriter.indentUnit);
}

void writeTreeMATLAB_LR(Token *root, TokenInfo const &tokenInfo,
                     TreeXMLWriter::Options const &options, std::string const &indentUnit,
                     std::ostream &out, ScannerType ) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  treeXMLWriter.writeMATLAB_LR_Stack(root, out, treeXMLWriter.indentUnit);
}

void writeTreeJSON(Token *root, TokenInfo const &tokenInfo,
                     TreeXMLWriter::Options const &options, std::string const &indentUnit,
                     std::ostream &out, ScannerType ) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  treeXMLWriter.writeJSON_Stack(root, out, treeXMLWriter.indentUnit);
}

std::ostream &configErrorStart(std::ostream &s, std::string const &fname, Token const *t) {
  s << fname << ":" << t->line << ":" << t->column << ":error: config: ";
  return s;
}

static std::string cnfFile;
static int cnfLine;
#define cnfls(code) (ls(code) << cnfFile << ":" << cnfLine << ": ")

bool parseOpCodeText(Lexer &lexer, Token const *t, ParserToken &opCode, std::string &opText) {
  opCode = TOKEN_EOF;
  opText = "";
  std::string const &ptext = t->text;
  if (t->token == TOKEN_STRING) {
    std::string name = ptext.substr(1, ptext.size() -2);
    if (name.empty()) {
      cnfls(LS::ERROR|LS::CONFIG) << "config: error: string cannot be empty\n";
      exit(EXIT_FAILURE);
    }
    cnfls(LS::DEBUG|LS::CONFIG) << "config: opdef name string " << name << "\n";
    lexer.setString(name);
    ParserToken oc = lexer.yylex();
    cnfls(LS::DEBUG|LS::CONFIG) << "config: opdef code " << oc << "\n";
    std::string ot = lexer.text();
    cnfls(LS::DEBUG|LS::CONFIG) << "config: opdef text " << ot << "\n";
    if (ot != name) {
      cnfls(LS::ERROR|LS::CONFIG) << "config: error: failed to parse the token completely: "
                               << name << " != " << oc << "(" << ot << ")\n";
      exit(EXIT_FAILURE);
    }
    opCode = oc;
    opText = ot;
  } else if (t->token == TOKEN_IDENTIFIER) {
    ParserToken token = Token::getParserTokenValue(ptext);
    cnfls(LS::DEBUG|LS::CONFIG) << "config: token:  " << token << "\n";
    opCode = token;
    // opText = name;
  } else {
    cnfls(LS::ERROR|LS::CONFIG) << "config: error: invalid token type field"
                             << t->token << " (not an identifier or string)\n";
    exit(EXIT_FAILURE);
  }
  return true;
}

void parseOperOptions(std::vector<Token const*> const &itemList,
                      ParserAssoc &assoc,
                      OutputMode &outputMode,
                      int &precedence,
                      int &unaryPrecedence) {

  int havePrec = 0;
  int haveAssoc = 0;
  int haveOMode = 0;

  for (unsigned int k = 2; k < 5 and k < itemList.size(); ++k) {
    cnfls(LS::DEBUG|LS::CONFIG) << "config: operator option: " << k << " " << *itemList[k] << "\n";
    if (itemList[k]->token == TOKEN_INTEGER) {
      int p = atoi(itemList[k]->text.c_str());
      if (p > 0) {
        cnfls(LS::DEBUG|LS::CONFIG) << "config: precedence " << havePrec << ":  " << p << "\n";
      } else {
        cnfls(LS::ERROR|LS::CONFIG) << "config: error: invalid precedence " << p << " (too small)\n";
      }
      if (havePrec == 0) {
        precedence = p;
      } else if (havePrec == 1) {
        unaryPrecedence = p;
      }
      ++havePrec;

    } else if (itemList[k]->token == TOKEN_IDENTIFIER) {
      int valid = 1;
      std::string const &modeName = itemList[k]->text;

      if (not haveAssoc) {
        ParserAssoc m = getParserAssocValue(modeName, &valid);
        if (valid == 0) {
          cnfls(LS::DEBUG|LS::CONFIG) << "config: assoc:  " << m << "\n";
          assoc = m;
          haveAssoc = true;
          continue;
        }
      }

      if (not haveOMode) {
        OutputMode o = getOutputModeValue(modeName, &valid);
        if (valid == 0) {
          cnfls(LS::DEBUG|LS::CONFIG) << "config: output-mode:  " << o << "\n";
          outputMode = o;
          haveOMode = true;
          continue;
        }
      }

      if (valid != 0) {
        cnfls(LS::ERROR|LS::CONFIG) << "config: error: invalid operator option string key '"
                                 << modeName << "'\n";
      }

    } else {
      cnfls(LS::ERROR|LS::CONFIG) << "error: config: invalid operator option field type '"
                               << itemList[k]->token << "' (not an integer or an identifier)\n";
    }
  }
}

ParserMode parseModeField(Token const *t) {
  ParserMode mode = MODE_IGNORE;
  if (t->token == TOKEN_IDENTIFIER) {
    std::string const &modeName = t->text;
    ParserMode m = getParserModeValue(modeName);
    cnfls(LS::DEBUG|LS::CONFIG) << "config: mode:  " << m << "\n";
    mode = m;
  } else {
    cnfls(LS::ERROR|LS::CONFIG) << "config: error: invalid mode field type " << t->token << " (not an identifier)\n";
  }
  return mode;
}

bool parseConfig(Lexer &lexer, std::string const &fname, Token const *t, TokenInfo &tokenInfo) {
  cnfFile = fname;
  cnfLine = 1;
  Token const *line = t->right;
  if (line == 0) {
    cnfls(LS::ERROR|LS::CONFIG) << "config: error: empty file\n";
    return false;
  }
  if (line->token != TOKEN_NEWLINE) {
    cnfls(LS::ERROR|LS::CONFIG) << "config: error: parsing error: root node not found\n";
    return false;
  }
  for (; line and line->token == TOKEN_NEWLINE; line = line->right) {
    ++cnfLine;
    cnfls(LS::DEBUG|LS::CONFIG) << "config: line " << *line << "\n";
    Token const *defHead = line->left;
    if (defHead and defHead->token == TOKEN_HASH) {
      cnfls(LS::DEBUG|LS::CONFIG) << "config: comment: line " << line->line << "\n";
      continue;
    } else if (defHead and defHead->token == TOKEN_JUXTA) {
      std::vector<Token const*> itemList;
      while (defHead and defHead->token == TOKEN_JUXTA) {
        Token const *item = defHead->left;
        cnfls(LS::DEBUG|LS::CONFIG) << ": config: item " << *item << "\n";
        itemList.push_back(item);
        defHead = defHead->right;
      }
      cnfls(LS::DEBUG|LS::CONFIG) << ": config: item " << *defHead << "\n";
      itemList.push_back(defHead);
      if (itemList.size() < 3) {
        cnfls(LS::ERROR|LS::CONFIG) << "config: error: line "
                                 << itemList.front()->line << " is too short (three items at least)\n";
      } else {

        // parse operator field
        ParserToken opCode = TOKEN_EOF;
        std::string opText;
        parseOpCodeText(lexer, itemList[0], opCode, opText);

        if (opCode == TOKEN_ROOT or opCode == TOKEN_EOF) {
          cnfls(LS::ERROR|LS::CONFIG) << "config: error: cannot set properties of token " << opCode << "\n";
          continue;
        }
        Token token(opCode, opText);

        // parse mode field
        ParserMode mode = parseModeField(itemList[1]);

        if (mode != MODE_PAREN) {
          
          cnfls(LS::DEBUG|LS::CONFIG) << "config: token:  " << token << "\n";

          // parse precedence field
          ParserAssoc assoc = mode == MODE_BINARY ? ASSOC_LEFT : ASSOC_NONE;
          OutputMode outputMode = OUTPUT_MODE_NESTED;
          int precedence = 100;
          int unary_precedence = 100;

          parseOperOptions(itemList, assoc, outputMode, precedence, unary_precedence);
          
          cnfls(LS::DEBUG|LS::CONFIG) << "config: options:  " << assoc << ", " << outputMode << ", " << precedence
                                   << ", " << unary_precedence << "\n";

          TokenProto p(token, precedence, mode, assoc);
          if (mode == MODE_UNARY_BINARY) {
            p.unaryPrecedence = unary_precedence;
          }
          p.outputMode = outputMode;
          cnfls(LS::DEBUG|LS::CONFIG) << "config: token:  " << p << "\n";

          if (itemList[0]->token == TOKEN_IDENTIFIER) {
            tokenInfo.prototypes[opCode] = p;
          } else if (opCode == TOKEN_IDENTIFIER) {
            tokenInfo.opPrototypes[tokenInfo.mkOpCode(opText)] = p;
          } else {
            tokenInfo.prototypes[opCode] = p;
          }

        } else {
          
          ParserToken eopCode = TOKEN_EOF;
          std::string eopText;
          parseOpCodeText(lexer, itemList[2], eopCode, eopText);

          if (opCode == TOKEN_IDENTIFIER and eopCode == TOKEN_IDENTIFIER) {
            tokenInfo.addBrace(opText, eopText);
          } else if (opCode == TOKEN_IDENTIFIER and eopCode != TOKEN_IDENTIFIER) {
            tokenInfo.addBrace(opText, eopCode);
          } else if (opCode != TOKEN_IDENTIFIER and eopCode == TOKEN_IDENTIFIER) {
            tokenInfo.addBrace(opCode, eopText);
          } else {
            tokenInfo.addBrace(opCode, eopCode);
          }

          if (itemList.size() > 3) {

            // shift list by two (FIXME: hack)
            std::vector<Token const*> subItemList(++ ++itemList.begin(), itemList.end());
            
            // parse sub mode field
            ParserMode subMode = parseModeField(subItemList[1]);
            cnfls(LS::DEBUG|LS::CONFIG) << "config: paren sub mode field:  " << subMode << "\n";

            // parse additional mode, precedence fields...
            ParserAssoc assoc = subMode == MODE_BINARY ? ASSOC_LEFT : ASSOC_NONE;
            OutputMode outputMode = OUTPUT_MODE_NESTED;
            int precedence = 100;
            int unary_precedence = 100;
            
            parseOperOptions(subItemList, assoc, outputMode, precedence, unary_precedence);

            TokenProto *tp = tokenInfo.getProto(&token);

            tp->mode = subMode;
            tp->precedence = precedence;
            tp->outputMode = outputMode;
            tp->associativity = assoc;

          }
        }

      }
    } else {
      if (defHead) {
        cnfls(LS::ERROR|LS::CONFIG) << "config: error: invalid line " << defHead->line << "\n";
      } else {
        cnfls(LS::DEBUG|LS::CONFIG) << "config: empty line " << line->line << "\n";
      }
    }
  }
  return true;
}

bool debugInit = false;

void printCopyright() {
  printf("Copyright (C) 2011-2016 Johannes Willkomm <johannes@johannes-willkomm.de>\n");
}

void printBugreportInfo() {
#ifdef PACKAGE_URL
  printf("Visit us on the web at %s\n", PACKAGE_URL);
#endif
  printf("Report bugs to %s\n", PACKAGE_BUGREPORT);
}

char const *vcs_version = LONG_VERSION;

void printVersion() {
  cmdline_parser_print_version();
  printCopyright();
}

void printHelp() {
  cmdline_parser_print_help();
  printf("\n");
  printBugreportInfo();
}

void printFullHelp() {
  cmdline_parser_print_full_help();
  printf("\n");
  printBugreportInfo();
}

#ifndef P2X_NO_MAIN
static bool fileExists(std::string const &filename) {
   FILE *hand;
   errno=0;                   // Delete old errors.
   hand= fopen(filename.c_str(), "r");      // Try to open dir
   if (hand) {                // A file can be opened
      fgetc(hand);            // and reading doesn't result in an error
      if (errno==0) {
         fclose(hand);
         return true;
      }                       // Not readable or problems. Report: It isn't a file.
      fclose(hand);
   }
   return false;
}

static gengetopt_args_info args;

static int readConfigFile(std::string const &fname) {
  if (not fileExists(fname)) {
    if (debugInit) {
      std::cerr << PACKAGE_NAME " config file does not exist\n";
    }
    return -1;
  }
  cmdline_parser_params cmdlineParams;
  cmdline_parser_params_init(&cmdlineParams);
  cmdlineParams.initialize = 0;
  cmdlineParams.override = true;
  cmdlineParams.check_required = 0;
  int res = cmdline_parser_config_file((char*)fname.c_str(), &args, &cmdlineParams);
  return res != 0;
}

int main(int argc, char *argv[]) {

  char const *dbInitVal = getenv("P2X_DEBUG_INIT");
  debugInit = dbInitVal != 0 and *dbInitVal != 0;
  if (debugInit) {
    std::cerr << "P2X_DEBUG_INIT is ON!\n";
  }

  std::string upUserDir;
#ifdef __WIN32
  std::string userHome = GetEnvString("HOMEDRIVE", "c:")() + GetEnvString("HOMEPATH", "\\")();
  std::string appDataDir = winGetFolderPath();
  if (appDataDir.empty()) {
    appDataDir = GetEnvString("APPDATA", userHome)();
  }
  upUserDir = GetEnvString("P2X_USER_DIR", appDataDir + "/" PACKAGE_NAME)();
#else
  std::string userHome = "~";
  userHome = GetEnvString("HOME", userHome)();
  upUserDir = GetEnvString("P2X_USER_DIR", userHome + "/." PACKAGE_NAME)();
#endif

  if (debugInit) {
    std::cerr << PACKAGE_NAME " user config directory: " << upUserDir << "\n";
  }

  cmdline_parser_init(&args);

  std::string configFileName = upUserDir + "/" PACKAGE_NAME "-options";
  if (debugInit) {
    std::cerr << "P2X user config file: " << configFileName << "\n";
  }

  int rc = readConfigFile(configFileName);
  if (rc == 1) {
    ls(LS::ERROR) << "invalid configuration file " << configFileName << "\n";
    exit(EXIT_FAILURE);
  }

  {
    cmdline_parser_params cmdlineParams;
    cmdline_parser_params_init(&cmdlineParams);
    cmdlineParams.initialize = 0;
    cmdlineParams.override = 1;
    cmdlineParams.check_required = 0;
    if (cmdline_parser_ext(argc, argv, &args, &cmdlineParams)!=0) {
      ls(LS::ERROR) << "invalid command line parameters\n";
      exit(EXIT_FAILURE);
    }
  }

  if (cmdline_parser_required(&args, argv[0]) != 0) {
    ls(LS::ERROR) << "invalid configuration\n";
    exit(EXIT_FAILURE);
  }

  if (args.debug_given>0) {
    LS::mask = 0xffff;
  }

  {
    int vLevel = 0, count = 0;
    if (args.verbose_given>0) {
      for (unsigned i = 0; i < args.verbose_given; ++i) {
        if (args.verbose_arg[i] != 0) {
          char *end = 0;
          int code = strtol(args.verbose_arg[i], &end, 0);
          if (end and *end == 0) {
            ls(LS::DEBUG)  << "Parsed arg of " << i << "-th -v flag as integer: " << code << "\n";
            vLevel |= code;
          } else {
            int ok;
            LS::LogLevel code = LS::getLogLevelValue(args.verbose_arg[i], &ok);
            if (ok == 0) {
              ls(LS::DEBUG)  << "Parsed arg of " << i << "-th -v flag as level name: " << code << "\n";
              vLevel |= code;
            } else {
              ls(LS::ERROR)  << "Arg of " << i << "-th -v flag is not an integer"
                " and not a valid log level name: " << args.verbose_arg[i] << "\n";
              exit(1);
            }
          }
        } else {
          vLevel |= 1 << (count + 2);
          ++count;
        }
      }
    }
    LS::mask |= vLevel;
    ls(LS::INFO)  << "Setting verbosity level to " << std::hex << "0x" << LS::mask << std::dec << "\n";
  }

  if (args.help_given) {
    printHelp();
    return EXIT_SUCCESS;
  }

  if (args.full_help_given) {
    printFullHelp();
    return EXIT_SUCCESS;
  }

  if (args.version_given) {
    printVersion();
    return EXIT_SUCCESS;
  }

  if (args.list_token_flag) {
    for (unsigned i = 0; i < Token::getNumParserToken(); ++i) {
      ParserToken t = ParserToken(Token::getParserToken(i));
      std::cout << (int(t)) << " " << Token::getParserTokenName(t) << "\n";
    }
    exit(EXIT_SUCCESS);
  }

  TreeXMLWriter::Options options;

  options.line = args.attribute_line_flag or args.src_info_flag;
  options.col = args.attribute_column_flag or args.src_info_flag;
  options._char = args.attribute_char_flag;

  options.mode = args.attribute_mode_flag;
  options.prec = args.attribute_precedence_flag;
  options.type = args.attribute_type_flag;
  options.id = args.attribute_id_flag;

  options.newlineAsBr = args.newline_as_br_flag;
  options.newlineAsEntity = args.newline_as_entity_flag;
  options.indent = args.indent_flag;
  options.merged = args.merged_flag;
  options.strict = args.strict_flag;
  options.writeRec = args.write_recursive_flag;

  std::string indentUnit = args.indent_unit_arg[0];
  if (args.indent_unit_given) {
    indentUnit = args.indent_unit_arg[args.indent_unit_given -1];
  }

  if (args.input_encoding_given>0) {
    options.encoding = args.input_encoding_arg[args.input_encoding_given -1];
  } else {
    options.encoding = args.input_encoding_arg[0];
  }


  std::ostream *_out = &std::cout;

  if (args.outfile_given) {
    _out = new std::ofstream(args.outfile_arg, std::ios::binary);
  }

  std::ostream &out = *_out;


  TokenInfo tokenInfo;

  ScannerType scannerType = SCANNER_NO_STRINGS; // the actual default is in p2x-opts.ggo
  {
    int scannerLastArg = 0;
    if (args.scanner_given>0) { // always true
      scannerLastArg = args.scanner_given -1;
    }
    std::string scannerTypeName = args.scanner_arg[scannerLastArg];
    int res;
    ScannerType s = getScannerTypeValue(scannerTypeName, &res);
    if (res == 0) {
      scannerType = s;
    } else {
      ls(LS::ERROR) << "error: Invalid scanner name '" << scannerTypeName << "', using default scanner "
                    << getScannerTypeName(scannerType)  << "\n";
    }
  }

  ls(LS::DEBUG|LS::CONFIG) << "args.inputs_num: " << args.inputs_num << std::endl;
  std::vector<std::string> fileList;
  for (unsigned i = 0 ; i < args.inputs_num; ++i ) {
    ls(LS::DEBUG|LS::CONFIG) << "arg (input file) given: " << args.inputs[i] << std::endl;
    fileList.push_back(args.inputs[i]);
  }



  {
    std::string precFileName, precPath;
    if (args.prec_list_given>0) {
      precFileName = args.prec_list_arg;
    }
    if (not precFileName.empty()) {
      precPath = upUserDir + "/" + precFileName;
      if (not fileExists(precPath)) {
        precPath = std::string(GetEnvString("P2X_CONFIG_DIR", ".")) + "/" + precFileName;
        if (not fileExists(precPath)) {
          precPath = precFileName;
        }
      }
      if (not fileExists(precPath)) {
        ls(LS::ERROR)  << "Failed to open config file " << precPath << ": " << strerror(errno) << "\n";
        exit(3);
      }
      FPParser configParser(SCANNER_STRINGS);
      configParser.tokenInfo.addIgnore(TOKEN_SPACE);
      configParser.tokenInfo.addBinary(TOKEN_NEWLINE, 10, ASSOC_RIGHT);
      configParser.tokenInfo.addUnary(TOKEN_HASH, 11);
      configParser.tokenInfo.addBinary(TOKEN_JUXTA, 12, ASSOC_RIGHT);
      // configParser.tokenInfo.addBinary("paren", 13, ASSOC_LEFT);
      configParser.options.ignoreIgnore = true;
      std::ifstream configFile(precPath.c_str());
      if (configFile) {
        ls(LS::INFO|LS::DEBUG)  << "Parsing config file " << precPath << "\n";
        Token *croot = configParser.parseStream(configFile);
        TreeXMLWriter::Options options;
        options.line = options.col = options.prec /* = options.sparse */ = 1;
        TreeXMLWriter treeXMLWriter(configParser.tokenInfo, options);
        // LS::mask |= LS::DEBUG;
        ls(LS::DEBUG|LS::CONFIG) << "Dumping XML of config file\n";
        treeXMLWriter.writeXML(croot, ls(LS::DEBUG|LS::CONFIG), "");
        Lexer lexer(scannerType);
        parseConfig(lexer, precPath, croot, tokenInfo);
        // LS::mask &= ~LS::DEBUG;
      } else {
        ls(LS::ERROR)  << "Failed to read config file " << precPath << ": " << strerror(errno) << "\n";
        exit(3);
      }
    }
  }

  if (args.binary_given>0) {
    int precedence = 1000;
    for (unsigned i = 0; i < args.binary_given; ++i) {
      std::string name = args.binary_arg[i];
      int ok;
      ParserToken tok = Token::getParserTokenValue(name, &ok);
      if (ok == 0) {
        switch(tok) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          break;
        default:
          tokenInfo.addBinary(tok, precedence+i);
          break;
        }
      } else {
        if ((name[0] == '\'' and name[name.size()-1] == '\'')
            or (name[0] == '"' and name[name.size()-1] == '"')) {
          name = name.substr(1, name.size()-2);
        }
        tokenInfo.addBinary(name, precedence+i);
      }
    }
  }

  if (args.right_given>0) {
    int precedence = 800;
    for (unsigned i = 0; i < args.right_given; ++i) {
      std::string name = args.right_arg[i];
      int ok;
      ParserToken tok = Token::getParserTokenValue(name, &ok);
      if (ok == 0) {
        switch(tok) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          break;
        default:
          tokenInfo.addBinary(tok, precedence+i, ASSOC_RIGHT);
          break;
        }
      } else {
        if ((name[0] == '\'' and name[name.size()-1] == '\'')
            or (name[0] == '"' and name[name.size()-1] == '"')) {
          name = name.substr(1, name.size()-2);
        }
        tokenInfo.addBinary(name, precedence+i, ASSOC_RIGHT);
      }
    }
  }

  if (args.unary_given>0) {
    int precedence = 2000;
    for (unsigned i = 0; i < args.unary_given; ++i) {
      std::string name = args.unary_arg[i];
      int ok;
      ParserToken tok = Token::getParserTokenValue(name, &ok);
      if (ok == 0) {
        switch(tok) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          break;
        default:
          tokenInfo.addUnary(tok, precedence+i);
          break;
        }
      } else {
        if ((name[0] == '\'' and name[name.size()-1] == '\'')
            or (name[0] == '"' and name[name.size()-1] == '"')) {
          name = name.substr(1, name.size()-2);
        }
        tokenInfo.addUnary(name, precedence+i);
      }
    }
  }

  if (args.postfix_given>0) {
    int precedence = 3000;
    for (unsigned i = 0; i < args.postfix_given; ++i) {
      std::string name = args.postfix_arg[i];
      int ok;
      ParserToken tok = Token::getParserTokenValue(name, &ok);
      if (ok == 0) {
        switch(tok) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          break;
        default:
          tokenInfo.addPostfix(tok, precedence+i);
          break;
        }
      } else {
        tokenInfo.addPostfix(name, precedence+i);
      }
    }
  }

  if (args.ignore_given>0) {
    for (unsigned i = 0; i < args.ignore_given; ++i) {
      std::string name = args.ignore_arg[i];
      int ok;
      ParserToken tok = Token::getParserTokenValue(name, &ok);
      if (ok == 0) {
        switch(tok) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          break;
        default:
          tokenInfo.addIgnore(tok);
          break;
        }
      } else {
        if ((name[0] == '\'' and name[name.size()-1] == '\'')
            or (name[0] == '"' and name[name.size()-1] == '"')) {
          name = name.substr(1, name.size()-2);
        }
        tokenInfo.addIgnore(name);
      }
    }
  }

  if (args.item_given>0) {
    for (unsigned i = 0; i < args.item_given; ++i) {
      std::string name = args.item_arg[i];
      int ok;
      ParserToken tok = Token::getParserTokenValue(name, &ok);
      if (ok == 0) {
        switch(tok) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          break;
        default:
          tokenInfo.addItem(tok);
          break;
        }
      } else {
        if ((name[0] == '\'' and name[name.size()-1] == '\'')
            or (name[0] == '"' and name[name.size()-1] == '"')) {
          name = name.substr(1, name.size()-2);
        }
        tokenInfo.addItem(name);
      }
    }
  }

  if (args.brace_given>0) {
    for (unsigned i = 0; i < args.brace_given; ++i) {
      std::string astr = args.brace_arg[i];
      size_t pc = astr.find_first_of(",;:");
      if (pc == std::string::npos) {
        ls(LS::ERROR|LS::PARSE) << "Parser: error: command line brace definition must be of the form start,end\n";
        exit(4);
      }
      std::string name1 = astr.substr(0, pc);
      std::string name2 = astr.substr(pc+1);
      int ok1, ok2;
      ParserToken tok1 = Token::getParserTokenValue(name1, &ok1);
      if (ok1 == 0) {
        switch(tok1) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          exit(5);
          break;
        default:
          break;
        }
      }
      ParserToken tok2 = Token::getParserTokenValue(name2, &ok2);
      if (ok2 == 0) {
        switch(tok2) {
        case TOKEN_EOF:
        case TOKEN_ROOT:
          exit(5);
          break;
        default:
          break;
        }
      }
      if (ok1 == 0 and ok2 == 0) {
        tokenInfo.addBrace(tok1, tok2);
      } else if (ok1 == 0) {
        tokenInfo.addBrace(tok1, name2);
      } else if (ok2 == 0) {
        tokenInfo.addBrace(name1, tok2);
      } else {
        tokenInfo.addBrace(name1, name2);
      }
    }
  }

  if (args.list_classes_given) {
    TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
    out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
    out << "<token-types xmlns='" NAMESPACE_CX "' xmlns:ca='" NAMESPACE_CA "'>\n";
    treeXMLWriter.writeXML(tokenInfo, out, indentUnit);
    out << "</token-types>\n";
    return 0;
  }


  // Start processing input
  std::istream *inStream = &std::cin, *infile = 0;
  if (not fileList.empty()) {
    infile = new std::ifstream(fileList[0].c_str());
    if (infile and *infile) {
      inStream = infile;
    } else {
      ls(LS::ERROR) << "Failed to open input file: " << fileList[0] << ": " << strerror(errno) << std::endl;
      return EXIT_FAILURE;
    }
  } else {
    if (isatty(0) and not args.stdin_tty_given) {
      ls(LS::ERROR) << "Refusing to read from the keyboard, use --stdin-tty to overrule" << std::endl;
      return EXIT_FAILURE;
    }
  }

  if (args.scan_only_given) {
    Timer tScanner;
    std::ostream &lout = ls(LS::TIMES) << "Scanning input... ";
    Scanner scanner(scannerType);
    scanner.readTokenList(*inStream);
    lout << "done in " << tScanner << " s" << std::endl;
    Timer tScanXML;
    std::ostream &lout2 = ls(LS::TIMES) << "Writing scan list to XML... ";
    TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
    out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
    out << "<scan-xml xmlns='" NAMESPACE_CX "' xmlns:ca='" NAMESPACE_CA "'>\n";
    treeXMLWriter.writeXML(scanner.tokenList, out, indentUnit);
    out << "</scan-xml>\n";
    lout2 << "done in " << tScanner << " s" << std::endl;
    return 0;
  }


  FPParser fpParser(scannerType);
  fpParser.tokenInfo = tokenInfo;

  Token::count = 0;

  Token *root = fpParser.parseStream(*inStream);

  Timer tXML;
  std::ostream &lout = ls(LS::TIMES) << "Writing tree to XML... ";

  std::string outputMode = "x";
  if (args.output_mode_given) {
    outputMode = args.output_mode_arg[0];
  }

  if (outputMode == "x")
    writeTreeXML(root, tokenInfo, options, indentUnit, out, scannerType);
  else if (outputMode == "y") {
    options.type = true;
    writeTreeXML2(root, tokenInfo, options, indentUnit, out, scannerType);
  } else if (outputMode == "j")
    writeTreeJSON(root, tokenInfo, options, indentUnit, out, scannerType);
  else if (outputMode == "m")
    writeTreeMATLAB(root, tokenInfo, options, indentUnit, out, scannerType);
  else if (outputMode == "n")
    writeTreeMATLAB_LR(root, tokenInfo, options, indentUnit, out, scannerType);

  lout << "done in " << tXML << " s" << std::endl;

  if (infile) {
    delete infile;
    infile = 0;
  }
  if (_out != &std::cout) {
    delete _out;
    _out = 0;
  }

  cmdline_parser_free(&args);
}
#endif

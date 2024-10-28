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

Copyright (C) 2011-2024 Johannes Willkomm

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
#define yyFlexLexer scanMFlexLexer
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
#include <algorithm>
#include <list>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <stdint.h>
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
    Log(LS::INFO, "GetFolderPath(APPDATA): " << result << "\n");
  } else {
    Log(LS::ERROR, "Failed to GetFolderPath(APPDATA)\n");
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

struct TokenProto : public Token {
  int precedence, unaryPrecedence;
  ParserMode mode;
  bool isParen;
  bool isRParen;
  bool ignoreIfStray;
  ParserAssoc associativity;
  OutputMode outputMode;
  typedef std::map<unsigned, TokenProto> EndList;
  EndList endList;
  std::set<ParserToken> ignoreAfter;
  TokenProto() :
    precedence(),
    unaryPrecedence(),
    mode(MODE_ITEM),
    isParen(),
    isRParen(),
    associativity(),
    outputMode()
  {}
  TokenProto(Token const &t, int precedence = 0,
             ParserMode mode = MODE_ITEM, ParserAssoc associativity = ASSOC_NONE,
             EndList const &endList = EndList(), int unaryPrecedence = 0) :
    Token(t),
    precedence(precedence),
    unaryPrecedence(unaryPrecedence),
    mode(mode),
    isParen(),
    isRParen(),
    ignoreIfStray(),
    associativity(associativity),
    outputMode(),
    endList(endList)
  {}
  void print(std::ostream &aus) const;
  bool operator == (TokenProto const &other) const {
    return not(*this != other);
  }
  bool operator != (TokenProto const &other) const {
    return mode != other.mode;
  }
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
    if (it->second.text.empty()) {
      out << it->second.token;
    } else {
      out << "'" << it->second.text << "'";
    }
  }
  out << "}";
  return out;
}

void TokenProto::print(std::ostream &aus) const {
  aus << "TokenProto(";
  Token::print(aus);
  aus << "," << precedence << "," << mode << "," << associativity << ", " << endList << ")";
}

struct LiveArity {
  Token const *m_s;
  LiveArity(Token const *s) : m_s(s) {}
  int operator()(Token const *s) const {
    if (s->left == 0 and s->right == 0) return 0;
    else if (s->left and s->right == 0) return 1;
    else if (s->left and s->right) return 2;
    else if (s->left == 0 and s->right) return 3;
    return -1;
  }
  operator int() const {
    return this->operator()(m_s);
  }
  bool operator ==(LiveArity const &o) const {
    return this->operator()(m_s) == o;
  }
};

struct TokenInfo {
  typedef std::map<std::string, unsigned> OpCodes;
  OpCodes opCodes;
  typedef std::map<unsigned, TokenProto> OpPrototypes;
  OpPrototypes opPrototypes;

  TokenInfo() {
    init();
  }

  void init() {
    initMandatory();
  }

  void initMandatory() {
    TokenProto tpj = TokenProto(Token(TOKEN_JUXTA, "j"), 900, MODE_BINARY, ASSOC_LEFT);
    tpj.outputMode = OUTPUT_MODE_NESTED;
    opPrototypes[TOKEN_JUXTA] = tpj;
  }

  unsigned mkOpCode(std::string const &s) {
    OpCodes::const_iterator it = opCodes.find(s);
    if (it != opCodes.end()) {
      return it->second;
    } else {
      unsigned const newCode = ceil((TOKEN_LAST + 1)/100.) * 100 + opCodes.size();
      Log(LS::INFO, "Generating new opcode for named operator: " << s << " -> " << newCode << "\n");
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
      Log(LS::ERROR|LS::PARSE, "Parser: error: precedence must be > 1\n");
      prec = 2;
    }
    TokenProto p(Token(TOKEN_IDENTIFIER, s), prec, MODE_BINARY, assoc);
    Log(LS::CONFIG|LS::INFO, "Adding binary named operator: " << p << "\n");
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addBinary(ParserToken t, int prec, ParserAssoc assoc = ASSOC_LEFT) {
    if (prec <= 1) {
      Log(LS::ERROR|LS::PARSE, "Parser: error: precedence must be > 1\n");
      prec = 2;
    }
    TokenProto p(Token(t, "unknown"), prec, MODE_BINARY, assoc);
    Log(LS::CONFIG|LS::INFO, "Adding binary operator: " << p << "\n");
    opPrototypes[t] = p;
    return true;
  }

  bool addUnary(std::string const &s, int prec) {
    if (prec <= 1) {
      Log(LS::ERROR|LS::PARSE, "Parser: error: precedence must be > 1\n");
      prec = 2;
    }
    TokenProto p(Token(TOKEN_IDENTIFIER, s), prec, MODE_UNARY);
    Log(LS::CONFIG|LS::INFO, "Adding unary named operator: " << p << "\n");
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addUnary(ParserToken t, int prec) {
    if (prec <= 1) {
      Log(LS::ERROR|LS::PARSE, "Parser: error: precedence must be > 1\n");
      prec = 2;
    }
    TokenProto p(Token(t, "unknown"), prec, MODE_UNARY);
    Log(LS::CONFIG|LS::INFO, "Adding unary operator: " << p << "\n");
    if (t == TOKEN_JUXTA) {
      Log(LS::ERROR|LS::PARSE, "error: Parser: cannot classify JUXTA as Unary\n");
    }
    opPrototypes[t] = p;
    return true;
  }

  bool addPostfix(std::string const &s, int prec) {
    if (prec <= 1) {
      Log(LS::ERROR|LS::PARSE, "Parser: error: precedence must be > 1\n");
      prec = 2;
    }
    TokenProto p(Token(TOKEN_IDENTIFIER, s), prec, MODE_POSTFIX);
    Log(LS::CONFIG|LS::INFO, "Adding postfix named operator: " << p << "\n");
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addPostfix(ParserToken t, int prec) {
    if (prec <= 1) {
      Log(LS::ERROR|LS::PARSE, "Parser: error: precedence must be > 1\n");
      prec = 2;
    }
    TokenProto p(Token(t, "unknown"), prec, MODE_POSTFIX);
    Log(LS::CONFIG|LS::INFO, "Adding postfix operator: " << p << "\n");
    if (t == TOKEN_JUXTA) {
      Log(LS::ERROR|LS::PARSE, "error: Parser: cannot classify JUXTA as Postfix\n");
    }
    opPrototypes[t] = p;
    return true;
  }

  bool addIgnore(std::string const &s) {
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_IGNORE);
    Log(LS::CONFIG|LS::INFO, "Adding named ignored item: " << p << "\n");
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addIgnore(ParserToken t) {
    TokenProto p(Token(t, "unknown"), 0, MODE_IGNORE);
    Log(LS::CONFIG|LS::INFO, "Adding ignored item: " << p << "\n");
    if (t == TOKEN_JUXTA) {
      Log(LS::ERROR|LS::PARSE, "error: Parser: cannot classify JUXTA as Ignore\n");
    }
    opPrototypes[t] = p;
    return true;
  }

  bool addItem(std::string const &s) {
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_ITEM);
    Log(LS::CONFIG|LS::INFO, "Adding named item: " << p << "\n");
    opPrototypes[mkOpCode(s)] = p;
    return true;
  }

  bool addItem(ParserToken t) {
    TokenProto p(Token(t, "unknown"), 0, MODE_ITEM);
    Log(LS::CONFIG|LS::INFO, "Adding item: " << p << "\n");
    if (t == TOKEN_JUXTA) {
      Log(LS::ERROR|LS::PARSE, "error: Parser: cannot classify JUXTA as Item\n");
    }
    opPrototypes[t] = p;
    return true;
  }

  bool addRBrace(TokenProto &tp) {
    tp.isRParen = 1;
    tp.mode = MODE_ITEM;
    unsigned const code = mkOpCode(tp);
    OpPrototypes::iterator it = opPrototypes.find(code);

    if (it != opPrototypes.end() && it->second != tp) {
      if (not it->second.isRParen) {
        Log(LS::CONFIG|LS::ERROR, "Overriding declaration of rbrace " << it->second << " with " << tp << "\n");
        exit(EXIT_FAILURE);
      }
      // else: ignore renewed rbrace declaration
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

  bool addBrace(unsigned const code, std::string const &name, TokenProto &tp) {
    tp.isParen = 1;
    tp.mode = MODE_ITEM;
    OpPrototypes::iterator it = opPrototypes.find(code);
    if (it != opPrototypes.end()) {
      if (not it->second.isParen) {
        Log(LS::CONFIG|LS::ERROR, "Overriding declaration of " << name << " as mode " << it->second.mode
            << " with mode " << MODE_PAREN << "\n")
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
  bool addBrace(std::string const &name, TokenProto &tp) {
    return addBrace(mkOpCode(name), name, tp);
  }
  bool addBrace(ParserToken t, TokenProto &tp) {
    return addBrace(t, Token::getParserTokenName(t), tp);
  }

  bool addBrace(std::string const &s, std::string const &send) {
    EndList el;
    el.insert(std::make_pair(mkOpCode(send), TokenProto(Token(TOKEN_IDENTIFIER, send))));
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_PAREN, ASSOC_NONE, el);
    Log(LS::CONFIG|LS::INFO, "Adding named brace: " << p << "\n");
    addBrace(s, p);
    return true;
  }
  bool addBrace(std::string const &s, ParserToken tend) {
    EndList el;
    el.insert(std::make_pair(tend, TokenProto(Token(tend, ""))));
    TokenProto p(Token(TOKEN_IDENTIFIER, s), 0, MODE_PAREN, ASSOC_NONE, el);
    Log(LS::CONFIG|LS::INFO, "Adding named brace: " << p << "\n");
    addBrace(s, p);
    return true;
  }
  bool addBrace(ParserToken t, std::string const &send) {
    EndList el;
    el.insert(std::make_pair(mkOpCode(send), TokenProto(Token(TOKEN_IDENTIFIER, send))));
    TokenProto p(Token(t, ""), 0, MODE_PAREN, ASSOC_NONE, el);
    Log(LS::CONFIG|LS::INFO, "Adding named brace: " << p << "\n");
    addBrace(t, p);
    return true;
  }
  bool addBrace(ParserToken t, ParserToken tend) {
    EndList el;
    el.insert(std::make_pair(tend, TokenProto(Token(tend, ""))));
    TokenProto p(Token(t, ""), 0, MODE_PAREN, ASSOC_NONE, el);
    Log(LS::CONFIG|LS::INFO, "Adding named brace: " << p << "\n");
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

  bool isAmbiguous(Token const * const t) const {
    OpPrototypes::const_iterator it = opPrototypes.find(getOpCode(t));
    return it != opPrototypes.end() && it->second.mode == MODE_UNARY_BINARY;
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
#ifdef P2X_SAVE_PROTO_PTR
    TokenProto const *res = t->proto;
    if (not res) {
#else
    TokenProto const *res = 0;
#endif
      OpPrototypes::const_iterator it = opPrototypes.find(getOpCode(t));
      if (it != opPrototypes.end()) {
        res = &it->second;
      }
#ifdef P2X_SAVE_PROTO_PTR
    }
#endif
    return res;
  }

  TokenProto *getProto(Token const * const t) {
    return const_cast<TokenProto*>(const_cast<TokenInfo const *>(this)->getProto(t));
  }

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
  bool isLParen(Token const * const t) const {
    bool res = false;
    TokenProto const *proto = getProto(t);
    if (proto) {
      res = proto->isParen;
    }
    return res;
  }
  bool isRParen(Token const * const t) const {
    bool res = false;
    TokenProto const *proto = getProto(t);
    if (proto) {
      res = proto->isRParen;
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

  bool canMerge(Token const * const t, bool globalMerge) const {
    bool res = false;
    TokenProto const *proto = getProto(t);
    if (proto) {
      res = (globalMerge or proto->outputMode == OUTPUT_MODE_MERGED)
        and (proto->mode == MODE_BINARY or (proto->mode == MODE_UNARY_BINARY and LiveArity(t) == 2));
    }
    return res;
  }

};

struct P2XLexer {
  P2XLexer() {}
  virtual ~P2XLexer() {}

  virtual void setStream(std::istream &ins) = 0;
  virtual void setString(std::string const &data) {
    m_istrstr.clear();
    m_istrstr.str(data);
    setStream(m_istrstr);
  }
  virtual ParserToken yylex() = 0;
  virtual std::string text() const = 0;

  std::istringstream m_istrstr;
};

struct P2XRE2CLexer : public P2XLexer {
  typedef unsigned char Re2cCharT;

  typedef int (*LexT)(const Re2cCharT *&, const Re2cCharT *&, Re2cCharT const*);

  LexT m_lexFunc;

  ScannerType m_type;

  Re2cCharT *m_buffer;
  size_t m_len;
  Re2cCharT const *m_current;
  Re2cCharT const *m_limit;

  std::string m_text;

  P2XRE2CLexer(LexT lexFunc, ScannerType type) : m_lexFunc(lexFunc), m_type(type), m_buffer() {
    Log(LS::CONFIG, "Creating Re2C lexer of type " << getScannerTypeName(m_type) << "\n");
  }

  ~P2XRE2CLexer() {
    clear();
  }

  void clear() {
    if (m_buffer) {
      delete[] m_buffer;
      m_buffer = 0;
    }
  }

  void setStream(std::istream &istr) {
    clear();
    std::string input;
    int const bufsz = 10000;
    char buffer[bufsz];
    size_t nread = 0;
    // Timer tScan;
    // std::ostream &lout = (Log(LS::TIMES, "Reading input..."));
    do {
      istr.read(buffer, bufsz);
      nread = istr.gcount();
      input += std::string(buffer, nread);
    } while (nread != 0);
    // lout << "done in " << tScan << " s" << std::endl;
    m_buffer = new Re2cCharT[input.size() + 1];
    memcpy(m_buffer, input.data(), input.size());
    m_len = input.size();
    m_buffer[input.size()] = 0;
    m_current = m_buffer;
    m_limit = m_buffer + input.size();
  }

  ParserToken yylex() {
    Re2cCharT const *input = m_current;
    Re2cCharT const *mark = 0;
    int c = m_lexFunc(input, mark, m_limit);
    if (c != TOKEN_EOF) {
      m_text.assign((char const *) m_current, input - m_current);
      m_len -= input - m_current;
      m_current = input;
    } else {
      m_text.clear();
    }
    return ParserToken(c);
  }

  std::string text() const {
    return m_text;
  }

};

struct P2XFlexLexer : public P2XLexer {
  ScannerType m_type;
  FlexLexer *m_flexLexer;

  P2XFlexLexer(ScannerType type = SCANNER_STRINGS) :
    m_type(type), m_flexLexer()
  {
    Log(LS::CONFIG, "Creating Flex lexer of type " << getScannerTypeName(m_type) << "\n");
    switch(type) {
    case SCANNER_FLEX_STRINGS:
      m_flexLexer = new scanFlexLexer();
      break;
    case SCANNER_FLEX_NO_STRINGS:
      m_flexLexer = new scanNSFlexLexer();
      break;
    case SCANNER_FLEX_R:
      m_flexLexer = new scanRFlexLexer();
      break;
    case SCANNER_FLEX_M:
      m_flexLexer = new scanMFlexLexer();
      break;
    case SCANNER_FLEX_C:
      m_flexLexer = new scanCFlexLexer();
      break;
    default:
      Log(LS::ERROR, "Flex lexer of type " << getScannerTypeName(m_type) << " is not available\n");
      break;
    }
  }

  ~P2XFlexLexer() {
    if (m_flexLexer) {
      delete m_flexLexer;
      m_flexLexer = 0;
    }
  }

  void setStream(std::istream &ins) {
    m_flexLexer->switch_streams(&ins, &std::cout);
  }
  ParserToken yylex() {
    return ParserToken(m_flexLexer->yylex());
  }
  std::string text() const {
    return m_flexLexer->YYText();
  }
};

extern int re2c_scanner_c_lex(P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*);
extern int re2c_scanner_r_lex(P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*);
extern int re2c_scanner_m_lex(P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*);
extern int re2c_scanner_strings_lex(P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*&, P2XRE2CLexer::Re2cCharT const*);

struct Lexer : public P2XLexer {
  ScannerType m_type;
  P2XLexer *m_p2xLexer;
  bool m_preferFlex;

  Lexer(ScannerType type_ = SCANNER_FLEX_STRINGS) :
    m_type(type_),
    m_p2xLexer(),
    m_preferFlex(getenv("P2X_PREFER_FLEX") != 0)
  {
    Log(LS::CONFIG, "Creating lexer of type " << getScannerTypeName(m_type) << "\n");
    switch (m_type) {
    case SCANNER_STRINGS:
      m_type = m_preferFlex ? SCANNER_FLEX_STRINGS : SCANNER_RE2C_STRINGS;
      break;
    case SCANNER_NO_STRINGS:
      m_type = SCANNER_FLEX_NO_STRINGS;
      break;
    case SCANNER_C:
      m_type = m_preferFlex ? SCANNER_FLEX_C : SCANNER_RE2C_C;
      break;
    case SCANNER_R:
      m_type = m_preferFlex ? SCANNER_FLEX_R : SCANNER_RE2C_R;
      break;
    case SCANNER_M:
      m_type = m_preferFlex ? SCANNER_FLEX_M : SCANNER_RE2C_M;
      break;
    default:
      break;
    }
    switch (m_type) {
    case SCANNER_STRINGS:
    case SCANNER_NO_STRINGS:
    case SCANNER_C:
    case SCANNER_R:
    case SCANNER_M:
      Log(LS::CONFIG, "Mapping lexer type " << getScannerTypeName(type_)
          << " to " << getScannerTypeName(m_type) << "\n");
    default:
      break;
    }
    switch(m_type) {
    case SCANNER_FLEX_STRINGS:
    case SCANNER_FLEX_NO_STRINGS:
    case SCANNER_FLEX_R:
    case SCANNER_FLEX_M:
    case SCANNER_FLEX_C:
      m_p2xLexer = new P2XFlexLexer(m_type);
      break;
    case SCANNER_RE2C_C:
      m_p2xLexer = new P2XRE2CLexer(re2c_scanner_c_lex, m_type);
      break;
    case SCANNER_RE2C_R:
      m_p2xLexer = new P2XRE2CLexer(re2c_scanner_r_lex, m_type);
      break;
    case SCANNER_RE2C_M:
      m_p2xLexer = new P2XRE2CLexer(re2c_scanner_m_lex, m_type);
      break;
    case SCANNER_RE2C_STRINGS:
      m_p2xLexer = new P2XRE2CLexer(re2c_scanner_strings_lex, m_type);
      break;
    default:
      Log(LS::ERROR, "Lexer of type " << getScannerTypeName(m_type) << " is not available\n");
      break;
    }
  }

  ~Lexer() {
    if (m_p2xLexer) {
      delete m_p2xLexer;
      m_p2xLexer = 0;
    }
  }

  void setStream(std::istream &ins) { m_p2xLexer->setStream(ins); }
  ParserToken yylex() { return m_p2xLexer->yylex(); }
  std::string text() const { return m_p2xLexer->text(); }
};

struct TokenTypeEqual {

  TokenInfo const &tokenInfo;

  TokenTypeEqual(TokenInfo const &tokenInfo) : tokenInfo(tokenInfo) {}

  bool operator()(Token const *s, Token const *t) const {
    return s->token == t->token
      and (not (tokenInfo.isNamedType(s) or tokenInfo.isNamedType(t)) or s->text == t->text)
      and (!(tokenInfo.isAmbiguous(s) or tokenInfo.isAmbiguous(t)) or LiveArity(s) == LiveArity(t));
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
      Log(LS::DEBUG|LS::SCAN, "token: " << tok << ": " << *n << "\n");
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

struct FileAndLine {
  struct Parser const &m_p;
  Token const &m_t;
  FileAndLine(Parser const &p, Token const &t) : m_p(p), m_t(t) {}
  FileAndLine(Parser const &p, Token const *t) : m_p(p), m_t(*t) {}
};

struct FileLineAndCol {
  struct Parser const &m_p;
  Token const &m_t;
  FileLineAndCol(Parser const &p, Token const &t) : m_p(p), m_t(t) {}
  FileLineAndCol(Parser const &p, Token const *t) : m_p(p), m_t(*t) {}
};

std::ostream &operator <<(std::ostream &aus, FileAndLine const &t);
std::ostream &operator <<(std::ostream &aus, FileLineAndCol const &t);

struct Parser {
  struct Options {
    bool ignoreIgnore;
    std::string inputFileName;
    Token *rootNode;
    Options() {
      ignoreIgnore = false;
    }
  };
  Token *root;
  TokenInfo const &tokenInfo;
  Options options;
  TokenList &tokenList;
  EndList endList;
  typedef std::map<int, Token*> LPrecMap;
  LPrecMap leastMap;
  Token *m_lastToken;

  Parser(TokenInfo const &tokenInfo, Options const &options, TokenList &tokenList) :
    root(), tokenInfo(tokenInfo), options(options), tokenList(tokenList), m_lastToken() {
    endList.insert(std::make_pair(unsigned(TOKEN_EOF), Token(TOKEN_EOF, "")));
  }

  ~Parser() {
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
    Log(LS::DEBUG|LS::PARSE, "rmop = " << it->second << " " << *it->second << " " << tokenInfo.prec(it->second) << "\n");
#endif
    assert(tokenInfo.mode(it->second) != MODE_ITEM);
    return it->second;
  }
  bool rightEdgeOpen() const {
    Token *rm = getRMOp();
    return rm->right == 0 and tokenInfo.mode(rm) != MODE_POSTFIX;
  }

  bool tokenTypeEqual(Token const *s, Token const *t) const {
    return TokenTypeEqual(tokenInfo)(s, t);
  }

  Token *mkRoot() {
    tokenList.tokenList.push_back(new Token(TOKEN_ROOT, ""));
    return tokenList.tokenList.back();
  }
  Token *mkJuxta(Token const * const t, ParserToken token = TOKEN_JUXTA) {
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
    Log(LS::DEBUG|LS::PARSE, "rmop prec = " << tokenInfo.prec(rmop) << "\n");
#endif
  }
  void pushItem(Token *t) {
    pushItem_(t);
#ifndef NDEBUG
    int const prec = INT_MAX; // == tokenInfo.prec(t);
    Log(LS::DEBUG|LS::PARSE, "prec = " << prec << "\n");
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
    Log(LS::DEBUG|LS::PARSE, "prec = " << prec << "\n");
    Log(LS::DEBUG|LS::PARSE, "Looking for " << prec << " in the map" << "\n");
    Log(LS::DEBUG|LS::PARSE, " assoc " << assoc << "" << "\n");
    {
      LPrecMap::iterator it = leastMap.begin();
      for (; it != leastMap.end(); ++it) {
        Log(LS::DEBUG|LS::PARSE, "Item: " << it->first << " " << it->second << "" << "\n");
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
      assert(t->left == 0);
      t->left = tmp;
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
      t->ignore = rm->ignore;
      rm->ignore = t;
    }
  }

  void insertToken(Token *first) {
#ifndef NDEBUG
    Log(LS::DEBUG|LS::PARSE, "insert Token: " << (void*)first << " " << *first << "\n");
#endif
    ParserMode firstMode = tokenInfo.mode(first);
    assert(not(firstMode & MODE_PAREN)); // MODE_PAREN bit is cleared
    assert(firstMode != 0); // mode is not 0
    assert(firstMode != MODE_PAREN); // mode is not MODE_PAREN
#ifndef NDEBUG
    Log(LS::DEBUG|LS::PARSE, "mode = " << firstMode << "\n");
#endif
    TokenProto const *tp = tokenInfo.getProto(first);
    if (firstMode == MODE_BINARY
        and tp->ignoreIfStray
        and rightEdgeOpen()) {
      firstMode = MODE_IGNORE;
    }
    if (m_lastToken and tp
        and tp->ignoreAfter.find(m_lastToken->token) != tp->ignoreAfter.end()) {
      firstMode = MODE_IGNORE;
    }
    switch(firstMode) {
    case MODE_ITEM:
      pushItem(first);
      break;
    case MODE_IGNORE:
      pushIgnore(first);
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
    case MODE_LINE_COMMENT:
      pushIgnore(first);
      break;
    case MODE_BLOCK_COMMENT:
      pushIgnore(first);
      break;
    default:
      Log(LS::ERROR|LS::PARSE, "Parser: error: invalid mode " << firstMode << "\n");
      exit(1);
      break;
    }
    m_lastToken = first;
  }

  Token *parse() {
    Token *first = 0;
#ifdef P2X_SAVE_PROTO_PTR
    TokenProto const* proto = 0;
#endif
    root = mkRoot();
    leastMap[tokenInfo.prec(root)] = root;
    bool endFound = false;
    do {
      // first = new Token(tokenList.next());
      first = tokenList.next();

      if (not first) {
        Log(LS::ERROR, "error: unexpected end of input token list\n");
        exit(4);
      }

#ifdef P2X_SAVE_PROTO_PTR
      proto = tokenInfo.getProto(first);
      first->proto = proto;
#endif

      Log(LS::DEBUG|LS::PARSE,  "Parser: next: " << *first
          << ": mode: " << tokenInfo.mode(first)
          << ": prec: " << tokenInfo.prec(first)
          << "\n");
      if (endList.find(tokenInfo.getOpCode(first)) != endList.end()) {
        endFound = true;
      } else if (first->token == TOKEN_EOF) {
        Log(LS::WARNING,  FileLineAndCol(*this, first) << ": Unexpected end of input in block "
            << TextLineAndCol(options.rootNode)
            << " while searching for " << endList << "\n");
        endFound = true;
      } else if (tokenInfo.mode(first) == MODE_LINE_COMMENT) {
        Token *next = 0;
        std::string lncText;
        while(true) {
          next = tokenList.next();
          if (next->token == TOKEN_NEWLINE or next->token == TOKEN_EOF)
            break;
          lncText += next->text;
        }
        insertToken(first);
        Token * const inserted = first;
        if (next->token == TOKEN_NEWLINE) {
          insertToken(next);
        } else if (next->token == TOKEN_EOF) {
          Log(LS::PARSE,  FileLineAndCol(*this, next) << ": Unexpected end of input in line comment "
              << TextLineAndCol(first) << " while searching for " << TOKEN_NEWLINE << "\n");
          endFound = true;
          first = next;
        }
        inserted->text += lncText;
      } else if (tokenInfo.mode(first) == MODE_BLOCK_COMMENT and tokenInfo.isLParen(first)) {
        Token *next = 0;
        auto pcommentEndList = tokenInfo.endList(first);
        std::string ctext = first->text;
        while(true) {
          next = tokenList.next();
          if (next->token == TOKEN_EOF)
            break;
          ctext += next->text;
          if (pcommentEndList.find(tokenInfo.getOpCode(next)) != pcommentEndList.end())
            break;
          if (tokenInfo.mode(next) == MODE_BLOCK_COMMENT) {
            Log(LS::WARNING,  FileLineAndCol(*this, next) << ": Block comment start inside block comment "
                << TextLineAndCol(first) << ", but nesting is not allowed\n");
          }
        }
        insertToken(first);
        Token * const inserted = first;
        if (next->token == TOKEN_EOF) {
          Log(LS::WARNING,  FileLineAndCol(*this, next) << ": Unexpected end of input in block comment "
              << TextLineAndCol(first) << " while searching for " << pcommentEndList << "\n");
          endFound = true;
          first = next;
        }
        inserted->fullText = ctext;
      } else if (tokenInfo.isLParen(first)) {
        Parser parser(tokenInfo, options, tokenList);
        parser.options.rootNode = first;
        parser.endList = tokenInfo.endList(first);
        Token *last = parser.parse();

        if (tokenInfo.assoc(first) != ASSOC_RIGHT) {
          if (last->token == TOKEN_EOF) {
            endFound = true;
            first->right = parser.root->right;
          } else {
            first->right = last;
            last->left = parser.root->right;
          }

          insertToken(first);

          if (first->right) {
            leastMap[tokenInfo.prec(first)] = first->right;
          }

          assert(last->right == 0);

        } else {

          if (last->token == TOKEN_EOF) {
            endFound = true;
            insertToken(first);
            first->right = parser.root->right;
            leastMap[tokenInfo.prec(first)] = first;
          } else {
            insertToken(last);

            assert(last->right == 0);

            first->left = last->left;
            last->left = first;
            first->right = parser.root->right;
            leastMap[tokenInfo.prec(first)] = last;
          }

        }

        if (parser.root->ignore) {
          assert(first->ignore == 0);
          first->ignore = parser.root->ignore;
        }

        first = last;

      } else {
        insertToken(first);
      }
    } while(not endFound);

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
    Log(LS::TIMES, "Scanning input...");
    scanner.readTokenList(ins);
    Log(LS::TIMES, "done in " << tScan << " s" << std::endl);
    TokenList tokenList(scanner.tokenList);
    Parser parser(tokenInfo, options, tokenList);
    {
      Timer tParse;
      Log(LS::TIMES, "Parsing input... ");
      parser.parse();
      Log(LS::TIMES, "done in " << tParse << " s" << std::endl);
      Log(LS::TIMES, "loaded in " << tAll << " s" << std::endl);
    }
    return parser.root;
  }

};

inline std::ostream &operator <<(std::ostream &aus, FileAndLine const &t) {
  aus << t.m_p.options.inputFileName << ":" << t.m_t.line;
  return aus;
}

inline std::ostream &operator <<(std::ostream &aus, FileLineAndCol const &t) {
  aus << t.m_p.options.inputFileName << ":" << t.m_t.line << ":" << t.m_t.column;
  return aus;
}

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

  int (HandlerClass::*enterFcn)(Token const *, Token const *);
  int (HandlerClass::*leaveFcn)(Token const *, Token const *);
  int (HandlerClass::*contentFcn)(Token const *, Token const *);

  TreeTraverser(HandlerClass *obj) :
    m_obj(obj),
    enterFcn(&HandlerClass::onEnter),
    leaveFcn(&HandlerClass::onLeave),
    contentFcn(&HandlerClass::onContent)
  {
  }

  void handleNode(StackItem const t) {
    int res = 0;
    switch (t.first) {
    case ST_ENTER:
      if (enterFcn) {
        res |= (m_obj->*enterFcn)(t.second, m_stack.sTop().second);
      }
      m_stack.sPush(std::make_pair(ST_BETWEEN, t.second));
      if (not(res & 1) && t.second->left) {
        m_stack.sPush(std::make_pair(ST_ENTER, t.second->left));
      }
      break;
    case ST_BETWEEN:
      if (contentFcn) {
        res |= (m_obj->*contentFcn)(t.second, m_stack.sTop().second);
      }
      m_stack.sPush(std::make_pair(ST_LEAVE, t.second));
      if (not(res & 2) && t.second->right) {
        m_stack.sPush(std::make_pair(ST_ENTER, t.second->right));
      }
      break;
    case ST_LEAVE:
      if (leaveFcn) {
        res |= (m_obj->*leaveFcn)(t.second, m_stack.sTop().second);
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

template<class HandlerClass>
void traverseTree(HandlerClass &handler, Token const *t) {
  TreeTraverser<HandlerClass> treeTraverser(&handler);
  treeTraverser.traverseTree(t);
}

struct Indent {
  std::string m_unit;
  std::string m_str;
  size_t      m_level;
  size_t      m_outlen;
  Indent(std::string const &unit = " ") : m_unit(unit), m_level(), m_outlen() {}
  ~Indent() {}
  void setLevel(size_t lev) {
    for (size_t k = m_str.size() / m_unit.size(); k < lev; ++k) {
      m_str += m_unit;
    }
    m_level = lev;
    m_outlen = lev * m_unit.size();
  }
  void print(std::ostream &aus) const {
    aus.write(m_str.c_str(), m_outlen);
  }
};

inline std::ostream &operator << (std::ostream &aus, Indent const &i) {
  i.print(aus);
  return aus;
}

struct TreeXMLWriter {
  struct Options {
    Options() :
      id(true), line(), col(), _char(),
      prec(), mode(), type(true),
      code(),
      indent(true),
      indentLogarithmic(true),
      newlineAsBr(true),
      newlineAsEntity(false),
      merged(),
      strict(),
      loose(),
      sparse(),
      xmlDecl(),
      bom(),
      scanConf(),
      parseConf(),
      treewriterConf(),
      caSteps(false),
      caVersion(false),
      commentVersion(false),
      writeRec(true),
      minStraightIndentLevel(135),
      encoding("default is in .ggo"),
      prefix_ca("ca"),
      prefix_ci("ci"),
      textTagName("text"),
      nullName("null")
    {}
    bool id, line, col, _char, prec, mode, type, code, indent, indentLogarithmic, newlineAsBr, newlineAsEntity, merged, strict, loose, sparse, xmlDecl, bom, scanConf, parseConf, treewriterConf, caSteps, caVersion, commentVersion, writeRec;
    unsigned minStraightIndentLevel;
    std::string encoding;
    std::string prefix_ca;
    std::string prefix_ci;
    std::string textTagName;
    std::string nullName;
  };
  TokenInfo const &tokenInfo;
  Options options;
  std::string indentUnit;
  std::string linebreak;
  std::string textTag;

  TreeXMLWriter (TokenInfo const &tokenInfo,
                 Options const &options,
                 std::string const &_indentUnit = " ",
                 std::string const &_linebreak = "\n") :
    tokenInfo(tokenInfo),
    options(options),
    textTag(options.prefix_ca + ":" + options.textTagName)
  {
    if (options.indent) {
      indentUnit = _indentUnit;
      linebreak = _linebreak;
    }
  }

  void writeXMLLocAttrs(Token const *t, std::ostream &aus) const {
    if (t->line) {
      if (options.line)
        aus << " line='" << t->line << "'";
      if (options.col)
        aus << " col='" << t->column << "'";
    }
    if (options._char)
      aus << " char='" << t->character << "'";
  }

  void writeXMLTypeElem(Token const *t, std::ostream &aus, std::string const &indent = "") const {
    aus << indent << indentUnit
        << "<ca:type id='" << int(t->token) << "' name='" << Token::getParserTokenName(t->token) << "'/>" << linebreak;
  }

  void writeXMLTypeAttrs(Token const *t, std::ostream &aus) const {
    if (t->token == TOKEN_IDENTIFIER) {
      if (options.code)
        aus << " code='" << tokenInfo.getOpCode(t) << "'";
      aus << " repr='" << t->text << "'";
    } else {
      if (options.code)
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

  void writeNEWLINE(Token const *t, std::ostream &aus, std::string const &tag="") const {
    if (options.newlineAsBr and not options.newlineAsEntity) {
      aus << "<" << options.prefix_ca << ":br/>";
    } else {
      if (tag.size()) aus << "<" << tag << ">";
      if (options.newlineAsEntity) {
        aus << "&#xa;";
      } else {
        aus << t->text;
      }
      if (tag.size()) aus << "</" << tag << ">";
    }
  }

  void writeCRETURN(Token const *t, std::ostream &aus, std::string const &tag="") const {
    if (options.newlineAsBr and not options.newlineAsEntity) {
      aus << "<" << options.prefix_ca << ":cr/>";
    } else {
      if (tag.size()) aus << "<" << tag << ">";
      if (options.newlineAsEntity) {
        aus << "&#xd;";
      } else {
        aus << t->text;
      }
      if (tag.size()) aus << "</" << tag << ">";
    }
  }

  bool writeXMLTextElem(Token const *t, std::ostream &aus) const {
    bool res = 0;
    if (t->token == TOKEN_NEWLINE) {
      writeNEWLINE(t, aus, textTag);
      res = 1;
    } else if (t->token == TOKEN_CRETURN) {
      writeCRETURN(t, aus, textTag);
      res = 1;
    } else if (t->text.size()) {
      aus << "<" << textTag << ">";
      XMLOstream x(aus);
      // ignore may end with NEWLINE, either because it is TOKEN_NEWLINE or it is a line comment
      if (t->text.back() == '\n') {
        x << t->text.substr(0, t->text.size()-1);
        if (options.newlineAsEntity)
          aus << "&#xa;";
        else if (options.newlineAsBr)
          ; // handled below
        else
          aus << "\n";
      } else if (t->fullText.size()) {
        x << t->fullText;
      } else {
        x << t->text;
      }
      aus << "</" << textTag << ">";
      if (options.newlineAsBr and t->text.back() == '\n')
        aus << "<ca:br/>";
      res = 1;
    }
    return res;
  }

  struct TreePrintHelper {
    TreeXMLWriter const &m_xmlWriter;
    size_t m_level;
    Indent indent, subindent;
    std::string elemName;
    bool merged, tags;
    std::ostream &aus;

    TreePrintHelper(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
      m_xmlWriter(xmlWriter),
      m_level(),
      indent(m_xmlWriter.indentUnit),
      subindent(m_xmlWriter.indentUnit),
      aus(aus)
    {}

    void setIndent() {
      if (m_xmlWriter.options.indent) {
        int ilevel = std::min<int>(m_level, m_xmlWriter.options.minStraightIndentLevel + log(std::max<size_t>(m_level, 1)));
#ifndef NDEBUG
        Log(LS::DEBUG|LS::PARSE, "rec. level -> indent level: " << m_level << " -> " << ilevel << "\n");
#endif
        indent.setLevel(ilevel);
        subindent.setLevel(ilevel+1);
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
      merged = m_xmlWriter.tokenInfo.canMerge(t, m_xmlWriter.options.merged);
    }

    int onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
      Log(LS::DEBUG|LS::PARSE, "parse: onEnter " << (void*)t << " " << *t << "\n");
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
        aus << ">";
        if (t->left || t->right || t->ignore) {
          aus << m_xmlWriter.linebreak;
          ++m_level;
        }
      }
      if (t->left == 0 and (t->right != 0 or !tags) and not m_xmlWriter.options.loose) {
        aus << (t->right != 0 and tags ? subindent : indent) << "<" << m_xmlWriter.options.nullName << "/>" << m_xmlWriter.linebreak;
      }

      return 0;
    }
    int onContent(Token const *t, Token const * parent) {
#ifndef NDEBUG
      Log(LS::DEBUG|LS::PARSE, "parse: onContent " << (void*)t << " " << *t << "\n");
#endif

      setupNode(t);
      setIndent();

      if (not t->text.empty()) {
        bool const tags = not(parent
                   and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                   and merged);
        bool const ownLine = t->left || t->right || t->ignore || !tags;
        if (ownLine)
          aus << indent;
        m_xmlWriter.writeXMLTextElem(t, aus);
        if (ownLine) aus << m_xmlWriter.linebreak;
      }
      if (t->ignore) {
        m_xmlWriter.writeIgnoreXML(t->ignore, aus, indent);
      }
      return 0;
    }
    int onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
      Log(LS::DEBUG|LS::PARSE, "parse: onLeave " << (void*)t << " " << *t << "\n");
#endif

      setupNode(t);
      setElemName(t);

      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged);
      if (t->right == 0) {
        if ((not tags or (m_xmlWriter.options.strict and
                          t->left != 0 and merged and TokenTypeEqual(m_xmlWriter.tokenInfo)(t, t->left))) and not m_xmlWriter.options.loose) {
          aus << indent << "<" << m_xmlWriter.options.nullName << "/>" << m_xmlWriter.linebreak;
        }
      }
      if (tags) {
        bool const ownLine = t->left || t->right || t->ignore;
        if (ownLine) {
          --m_level;
          setIndent();
          aus << indent;
        }
        aus << "</" << elemName << ">" << m_xmlWriter.linebreak;
      }
      return 0;
    }
  };

  struct XMLTreePrintHelper2 {
    TreeXMLWriter const &m_xmlWriter;
    size_t m_level;
    Indent indent, subindent;
    std::string elemName;
    bool merged, tags;
    std::ostream &aus;
    XMLOstream xaus;

    XMLTreePrintHelper2(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
      m_xmlWriter(xmlWriter),
      m_level(),
      indent(m_xmlWriter.indentUnit),
      subindent(m_xmlWriter.indentUnit),
      aus(aus),
      xaus(aus)
    {}

    void setIndent() {
      if (m_xmlWriter.options.indent) {
        int ilevel = std::min<int>(m_level, m_xmlWriter.options.minStraightIndentLevel + log(std::max<size_t>(m_level, 1)));
#ifndef NDEBUG
        Log(LS::DEBUG|LS::PARSE, "rec. level -> indent level: " << m_level << " -> " << ilevel << "\n");
#endif
        indent.setLevel(ilevel);
        subindent.setLevel(ilevel+1);
      }
    }

  void writeSingleTokenTextNode(Token const *t) {
    if (t->token == TOKEN_NEWLINE) {
      if (m_xmlWriter.options.newlineAsBr) {
        aus << "<c:br/>";
      } else {
        if (m_xmlWriter.options.newlineAsEntity) {
          aus << "&#xa;";
        } else {
          aus << t->text;
        }
      }
    } else if (t->token == TOKEN_CRETURN) {
      if (m_xmlWriter.options.newlineAsBr) {
        aus << "<c:cr/>";
      } else {
        if (m_xmlWriter.options.newlineAsEntity) {
          aus << "&#xd;";
        } else {
          aus << t->text;
        }
      }
    } else if (t->fullText.size()) {
      xaus << t->fullText;
    } else if (t->text.size()) {
      xaus << t->text;
    }
  }

  void writeIgnoreXML(Token const *t, Indent const &indent) {
    if (t->ignore) {
      writeIgnoreXML(t->ignore, indent);
    }
    aus << indent << "<" << m_xmlWriter.options.prefix_ci << ":" << Token::getParserTokenName(t->token);
    if (m_xmlWriter.options.id)
      aus << " id='" << t->id << "'";
    m_xmlWriter.writeXMLLocAttrs(t, aus);
    // writeXMLTypeAttrs(t, aus);
    aus << ">";
    writeSingleTokenTextNode(t);
    aus << "</" << m_xmlWriter.options.prefix_ci << ":" << Token::getParserTokenName(t->token) << ">" << m_xmlWriter.linebreak;
  }

  bool writeXMLTextElem(Token const *t) {
    bool res = 0;
    if (t->token != TOKEN_NEWLINE and t->token != TOKEN_CRETURN
        and not t->text.empty())
      aus << "<" << m_xmlWriter.textTag << ">";
    writeSingleTokenTextNode(t);
    if (t->token != TOKEN_NEWLINE and t->token != TOKEN_CRETURN
        and not t->text.empty())
      aus << "</" << m_xmlWriter.textTag << ">";
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

    void setupNode(Token const *t, Token const *parent) {
      merged = m_xmlWriter.tokenInfo.canMerge(t, m_xmlWriter.options.merged);
      tags = not(parent
                 and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                 and merged);
    }

    int onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
      Log(LS::DEBUG|LS::PARSE, "parse: onEnter " << (void*)t << " " << *t << "\n");
#endif
      setupNode(t, parent);
      setElemName(t);
      setIndent();

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
      if (t->left == 0 and (t->right != 0 or !tags) and not m_xmlWriter.options.loose) {
        aus << (tags ? subindent : indent) << "<" << m_xmlWriter.options.nullName << "/>" << m_xmlWriter.linebreak;
      }

      return 0;
    }
    int onContent(Token const *t, Token const *parent) {
#ifndef NDEBUG
      Log(LS::DEBUG|LS::PARSE, "parse: onContent " << (void*)t << " " << *t << "\n");
#endif

      setupNode(t, parent);
      setIndent();

      if (not t->text.empty()) {
        bool const ownLine = t->left || t->right || t->ignore || !tags;
        if (ownLine)
          aus << indent;
        writeXMLTextElem(t);
        if (ownLine) aus << m_xmlWriter.linebreak;
      }
      if (t->ignore) {
        writeIgnoreXML(t->ignore, indent);
      }
      return 0;
    }
    int onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
      Log(LS::DEBUG|LS::PARSE, "parse: onLeave " << (void*)t << " " << *t << "\n");
#endif

      setupNode(t, parent);
      setElemName(t);

      if (t->right == 0) {
        if ((not tags or (m_xmlWriter.options.strict and
                          t->left != 0 and merged and TokenTypeEqual(m_xmlWriter.tokenInfo)(t, t->left))) and not m_xmlWriter.options.loose) {
          aus << indent << "<" << m_xmlWriter.options.nullName << "/>" << m_xmlWriter.linebreak;
        }
      }
      if (tags) {
        if (t->left or t->right or t->ignore) {
          --m_level;
          setIndent();
          aus << indent;
        }
        aus << "</" << elemName << ">" << m_xmlWriter.linebreak;
      }
      return 0;
    }
  };

  #include "TreePrintHelperMatlabLR.hh"
  #include "TreePrintHelperMatlabChildren.hh"
  #include "TreePrintHelperJSONChildren.hh"

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

    traverseTree(printer, t);
  }
  void writeXML_Stack(Token const *t, std::ostream &aus,
                std::string const & = "", Token const * = 0,
                int level = 0) const {

    TreePrintHelper printer(*this, aus);
    printer.m_level = level + 1;

    traverseTree(printer, t);
  }
  template<class TreePrintHelperMATLAB>
  void TwriteMATLAB_Stack(Token const *t, std::ostream &aus,
                std::string const & = "", Token const * = 0,
                int level = 0) const {

    TreePrintHelperMATLAB printer(*this, aus);
    printer.m_level = level + 1;

    traverseTree(printer, t);
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

    TreePrintHelperJSONChildren printer(*this, aus);
    printer.m_level = level + 1;

    traverseTree(printer, t);
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

    bool const merged = tokenInfo.canMerge(t, options.merged);
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
      aus << ">";
    }
    bool const caTextOnNewLine = !options.sparse || t->left || t->right || t->ignore;
    if (caTextOnNewLine) {
      aus << linebreak;
    }
    if (t->left != 0) {
      writeXML_Rec(t->left, aus, subindent, t, level+1);
    } else if (t->right != 0 and not options.loose) {
      aus << indent << indentUnit << "<" << options.nullName << "/>" << linebreak;
    }
    if (not t->text.empty()) {
      if (caTextOnNewLine) {
        aus << subindent;
      }
      writeXMLTextElem(t, aus);
      if (caTextOnNewLine) {
        aus << linebreak;
      }
    }
    if (t->ignore) {
      writeIgnoreXML(t->ignore, aus, subindent);
    }
    if (t->right != 0) {
      writeXML_Rec(t->right, aus, subindent, t, level+1);
    } else if ((not tags or (options.strict and
                             t->left != 0 and merged and TokenTypeEqual(tokenInfo)(t, t->left))) and not options.loose) {
      aus << indent << "<" << options.nullName << "/>" << linebreak;
    }
    if (tags) {
      if (caTextOnNewLine) {
        aus << indent;
      }
      aus << "</" << elemName << ">" << linebreak;
    }
  }

  void writeIgnoreXML(Token *t, std::ostream &aus, Indent const &indent) const {
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

  void writeXML(EndList const &endList, std::ostream &aus, std::string const &indent = "", std::string const &ns = "ca") const {
    aus << indent << "<" << ns << ":closing-list>" << linebreak;
    for (EndList::const_iterator eit = endList.begin(); eit != endList.end(); ++eit) {
    writeXML(eit->second, aus, indent + indentUnit, ns);
    }
    aus << indent << "</" << ns << ":closing-list>" << linebreak;
  }

  void writeXML(TokenProto const &tp, std::ostream &aus, std::string const &indent = "", std::string const &ns = "ca") const {
    aus << indent << "<" << ns << ":op";
    writeXMLTypeAttrs(&tp, aus);
    aus << " mode='" << tp.mode << "'";
    if (Parser::isOp(tp.mode)) {
      aus << " precedence='" << tp.precedence << "'";
      aus << " associativity='" << tp.associativity << "'";
      if (tp.outputMode)
        aus << " output-mode='" << getOutputModeName(tp.outputMode) << "'";
      if (tp.ignoreIfStray)
        aus << " ignore-if-stray='1'";
    }
    if (tp.mode == MODE_UNARY_BINARY) {
      aus << " unary-precedence='" << tp.unaryPrecedence << "'";
    }
    if (tp.isRParen) {
      aus << " is-rparen='1'";
    }
    if (tp.isParen) {
      aus << " is-paren='1'";
      aus << ">" << linebreak;
      writeXML(tp.endList, aus, indent + indentUnit, ns);
      aus << indent << "</" << ns << ":op>" << linebreak;
    } else {
      aus << "/>" << linebreak;
    }
  }

  void writeXML(TokenInfo const &t, std::ostream &aus, std::string const &indent = "", std::string const &ns = "ca") const {
    aus << indent << "<" << ns << ":parser>" << linebreak;
    TokenInfo::OpPrototypes::const_iterator it = t.opPrototypes.begin();
    for(; it != t.opPrototypes.end(); ++it) {
    writeXML(it->second, aus, indent + indentUnit, ns);
    }
    aus << indent << "</" << ns << ":parser>" << linebreak;
  }

  void writeXML(TreeXMLWriter const &t, std::ostream &aus, std::string const &indent = "", std::string const &ns = "ca") const {
    aus << indent << "<" << ns << ":tree-writer";
    aus << " col='" << t.options.col << "'";
    aus << " merged='" << t.options.merged << "'";
    aus << " encoding='" << t.options.encoding << "'";
    aus << " nullName='" << t.options.nullName << "'";
    aus << " id='" << t.options.id << "'";
    aus << " indent='" << t.options.indent << "'";
    aus << " line='" << t.options.line << "'";
    aus << " mode='" << t.options.mode << "'";
    aus << " newlineAsBr='" << t.options.newlineAsBr << "'";
    aus << " newlineAsEntity='" << t.options.newlineAsEntity << "'";
    aus << " prec='" << t.options.prec << "'";
    aus << " strict='" << t.options.strict << "'";
    aus << " loose='" << t.options.loose << "'";
    aus << " sparse='" << t.options.sparse << "'";
    aus << " type='" << t.options.type << "'";
    aus << " xml-decl='" << t.options.xmlDecl << "'";
    aus << "/>" << linebreak;
  }

  void writeXML(std::vector<Token *> const &t, std::ostream &aus, std::string const &indent = "") const {
    std::vector<Token *>::const_iterator it = t.begin();
    for(; it != t.end(); ++it) {
      writeXML(*it, aus, indent);
    }
  }

};

static std::string getCopyright() {
  return "Copyright (C) 2011-2024 Johannes Willkomm <johannes@johannes-willkomm.de>";
}

char const *vcs_version = VCS_REVISION;

void writeVersionInfoXMLComment(TreeXMLWriter::Options const &, std::string const &, std::ostream &out) {
  out << "<!-- P2X version " << PACKAGE_VERSION << " (" << std::string(vcs_version).substr(0,8).c_str() << ") -->";
  //  out << "<!-- " << getCopyright() << " -->\n";
}

void writeVersionInfoXML(TreeXMLWriter::Options const &opts, std::string const &indent, std::ostream &out) {
  if (opts.indent)
    out << indent;
  std::string vs = PACKAGE_VERSION, major, minor, patch;
  size_t pos = 0;
  if ((pos = vs.find(".")) != std::string::npos) {
    major = vs.substr(0, pos);
    vs.erase(0, pos + 1);
  }
  if ((pos = vs.find(".")) != std::string::npos) {
    minor = vs.substr(0, pos);
    vs.erase(0, pos + 1);
  }
  patch = vs;
  out << "<" << opts.prefix_ca << ":version"
      << " name='P2X'"
      << " id='" << std::string(vcs_version).substr(0,8) << "'"
      << " major='" << major << "' minor='" << minor << "' patch='" << patch << "'>";
  writeVersionInfoXMLComment(opts, indent, out);
  out << "</" << opts.prefix_ca << ":version>";
}

void writeTreeXML(Token *root, TokenInfo const &tokenInfo,
                  TreeXMLWriter::Options const &options, std::string const &indentUnit,
                  std::ostream &out, ScannerType scannerType) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  if (options.xmlDecl) {
    out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
  }
  if (options.commentVersion) {
    writeVersionInfoXMLComment(options, indentUnit, out);
    out << treeXMLWriter.linebreak;
  }
  out << "<code-xml xmlns='" NAMESPACE_CX "' xmlns:ca='" NAMESPACE_CA "' ca:version='1.0'>" << treeXMLWriter.linebreak;
  if (options.caVersion) {
    writeVersionInfoXML(options, indentUnit, out);
    out << treeXMLWriter.linebreak;
  }
  if (options.caSteps)
    out << treeXMLWriter.indentUnit << "<ca:steps/>" << treeXMLWriter.linebreak;
  if (options.scanConf) {
    out << treeXMLWriter.indentUnit << "<ca:scanner type='"
        << getScannerTypeName(scannerType) << "'/>" << treeXMLWriter.linebreak;
  }
  if (options.treewriterConf) {
    treeXMLWriter.writeXML(treeXMLWriter, out, treeXMLWriter.indentUnit);
  }
  if (options.parseConf) {
    treeXMLWriter.writeXML(tokenInfo, out, treeXMLWriter.indentUnit);
  }
  treeXMLWriter.writeXML(root, out, treeXMLWriter.indentUnit);
  out << "</code-xml>\n";
}

void writeTreeXML2(Token *root, TokenInfo const &tokenInfo,
                   TreeXMLWriter::Options const &options, std::string const &indentUnit,
                   std::ostream &out, ScannerType scannerType) {
  TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
  if (options.xmlDecl) {
    out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
  }
  if (options.commentVersion) {
    writeVersionInfoXMLComment(options, indentUnit, out);
    out << treeXMLWriter.linebreak;
  }
  out << "<code-xml xmlns='" NAMESPACE_CX "' xmlns:" << options.prefix_ca << "='" NAMESPACE_CA "'"
    " xmlns:" << options.prefix_ci << "='" NAMESPACE_CX "ignore/'>" << treeXMLWriter.linebreak;
  if (options.caVersion) {
    writeVersionInfoXML(options, indentUnit, out);
    out << treeXMLWriter.linebreak;
  }
  if (options.scanConf) {
    out << treeXMLWriter.indentUnit << "<c:scanner type='"
        << getScannerTypeName(scannerType) << "'/>" << treeXMLWriter.linebreak;
  }
  if (options.treewriterConf) {
    treeXMLWriter.writeXML(treeXMLWriter, out, treeXMLWriter.indentUnit, "c");
  }
  if (options.parseConf) {
    treeXMLWriter.writeXML(tokenInfo, out, treeXMLWriter.indentUnit, "c");
  }
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

bool parseOpCodeText(Lexer &lexer, Token const *t, ParserToken &opCode, std::string &opText) {
  opCode = TOKEN_EOF;
  opText = "";
  std::string const &ptext = t->text;
  if (t->token == TOKEN_STRING) {
    std::string name = ptext.substr(1, ptext.size() -2);
    if (name.empty()) {
      Log(LS::ERROR|LS::CONFIG, "config: error: string cannot be empty\n");
      exit(EXIT_FAILURE);
    }
    Log(LS::DEBUG|LS::CONFIG, "config: opdef name string " << name << "\n");
    lexer.setString(name);
    ParserToken oc = lexer.yylex();
    Log(LS::DEBUG|LS::CONFIG, "config: opdef code " << oc << "\n");
    std::string ot = lexer.text();
    Log(LS::DEBUG|LS::CONFIG, "config: opdef text " << ot << "\n");
    if (ot != name) {
      Log(LS::ERROR|LS::CONFIG, "config: error: failed to parse the token completely: "
          << name << " != " << oc << "(" << ot << ")\n")
      exit(EXIT_FAILURE);
    }
    opCode = oc;
    opText = ot;
  } else if (t->token == TOKEN_IDENTIFIER) {
    ParserToken token = Token::getParserTokenValue(ptext);
    Log(LS::DEBUG|LS::CONFIG, "config: token:  " << token << "\n");
    opCode = token;
    // opText = name;
  } else {
    Log(LS::ERROR|LS::CONFIG, "config: error: invalid token type field"
        << t->token << " (not an identifier or string)\n")
    exit(EXIT_FAILURE);
  }
  return true;
}

void parseOperOptions(std::vector<Token const*> const &itemList,
                      ParserAssoc &assoc,
                      OutputMode &outputMode,
                      bool &ignoreIfStray,
                      int &precedence,
                      int &unaryPrecedence) {

  int havePrec = 0;
  int haveAssoc = 0;
  int haveOMode = 0;

  for (unsigned int k = 2; k < 5 and k < itemList.size(); ++k) {
    Log(LS::DEBUG|LS::CONFIG, "config: operator option: " << k << " " << *itemList[k] << "\n");
    if (itemList[k]->token == TOKEN_INTEGER) {
      int p = atoi(itemList[k]->text.c_str());
      if (p > 0) {
        Log(LS::DEBUG|LS::CONFIG, "config: precedence " << havePrec << ":  " << p << "\n");
      } else {
        Log(LS::ERROR|LS::CONFIG, "config: error: invalid precedence " << p << " (too small)\n");
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
          Log(LS::DEBUG|LS::CONFIG, "config: assoc:  " << m << "\n");
          assoc = m;
          haveAssoc = true;
          continue;
        }
      }

      if (not haveOMode) {
        OutputMode o = getOutputModeValue(modeName, &valid);
        if (valid == 0) {
          Log(LS::DEBUG|LS::CONFIG, "config: output-mode:  " << o << "\n");
          outputMode = o;
          haveOMode = true;
          continue;
        }
      }

      if (modeName == "ignoreIfStray") {
        ignoreIfStray = true;
        continue;
      }

      if (valid != 0) {
        Log(LS::ERROR|LS::CONFIG, "config: error: invalid operator option string key '" << modeName << "'\n");
          }

    } else {
      Log(LS::ERROR|LS::CONFIG, "error: config: invalid operator option field type '"
          << itemList[k]->token << "' (not an integer or an identifier)\n")
    }
  }
}

ParserMode parseModeField(Token const *t) {
  ParserMode mode = MODE_IGNORE;
  if (t->token == TOKEN_IDENTIFIER) {
    std::string const &modeName = t->text;
    ParserMode m = getParserModeValue(modeName);
    Log(LS::DEBUG|LS::CONFIG, "config: mode:  " << m << "\n");
    mode = m;
  } else {
    Log(LS::ERROR|LS::CONFIG, "config: error: invalid mode field type " << t->token << " (not an identifier)\n");
  }
  return mode;
}

bool parseConfig(Lexer &lexer, std::string const &fname, Token const *t, TokenInfo &tokenInfo) {
  cnfFile = fname;
  cnfLine = 1;
  Token const *line = t->right;
  if (line == 0) {
    Log(LS::ERROR|LS::CONFIG, "config: error: empty file\n");
    return false;
  }
  if (line->token != TOKEN_NEWLINE) {
    Log(LS::ERROR|LS::CONFIG, "config: error: parsing error: root node not found\n");
    return false;
  }
  for (; line and line->token == TOKEN_NEWLINE; line = line->right) {
    ++cnfLine;
    Log(LS::DEBUG|LS::CONFIG, "config: line " << *line << "\n");
    Token const *defHead = line->left;
    if (defHead and defHead->token == TOKEN_HASH) {
      Log(LS::DEBUG|LS::CONFIG, "config: comment: line " << line->line << "\n");
      continue;
    } else if (defHead and defHead->token == TOKEN_JUXTA) {
      std::vector<Token const*> itemList;
      while (defHead and defHead->token == TOKEN_JUXTA) {
        Token const *item = defHead->left;
        Log(LS::DEBUG|LS::CONFIG, ": config: item " << *item << "\n");
        itemList.push_back(item);
        defHead = defHead->right;
      }
      Log(LS::DEBUG|LS::CONFIG, ": config: item " << *defHead << "\n");
      itemList.push_back(defHead);
      if (itemList.size() < 3) {
        Log(LS::ERROR|LS::CONFIG, "config: error: line "
            << itemList.front()->line << " is too short (three items at least)\n")
      } else {

        // parse operator field
        ParserToken opCode = TOKEN_EOF;
        std::string opText;
        parseOpCodeText(lexer, itemList[0], opCode, opText);

        if (opCode == TOKEN_ROOT or opCode == TOKEN_EOF) {
          Log(LS::ERROR|LS::CONFIG, "config: error: cannot set properties of token " << opCode << "\n");
          continue;
        }
        Token token(opCode, opText);

        if (itemList[1]->text == "ignoreAfter") {
          ParserToken opCode2 = TOKEN_EOF;
          std::string opText2;
          parseOpCodeText(lexer, itemList[2], opCode2, opText2);
          if (tokenInfo.getProto(&token) == 0) {
            Log(LS::ERROR|LS::CONFIG, "config: error: Token " << opText << " must be defined first\n");
          } else {
            tokenInfo.getProto(&token)->ignoreAfter.insert(opCode2);
          }
          continue;
        }

        // parse mode field
        ParserMode mode = parseModeField(itemList[1]);

        if (mode != MODE_PAREN and mode != MODE_BLOCK_COMMENT) {

          Log(LS::DEBUG|LS::CONFIG, "config: token:  " << token << "\n");

          // parse precedence field
          ParserAssoc assoc = mode == MODE_BINARY or mode == MODE_UNARY_BINARY ? ASSOC_LEFT : ASSOC_NONE;
          OutputMode outputMode = OUTPUT_MODE_NESTED;
          int precedence = 100;
          int unary_precedence = 100;
          bool ignoreIfStray = false;

          parseOperOptions(itemList, assoc, outputMode, ignoreIfStray, precedence, unary_precedence);

          Log(LS::DEBUG|LS::CONFIG, "config: options:  " << assoc << ", " << outputMode << ", " << precedence
              << ", " << unary_precedence << "\n")

          TokenProto p(token, precedence, mode, assoc);
          if (mode == MODE_UNARY_BINARY) {
            p.unaryPrecedence = unary_precedence;
          }
          p.outputMode = outputMode;
          p.ignoreIfStray = ignoreIfStray;
          Log(LS::DEBUG|LS::CONFIG, "config: token:  " << p << "\n");

          if (itemList[0]->token == TOKEN_IDENTIFIER) {
            tokenInfo.opPrototypes[opCode] = p;
          } else if (opCode == TOKEN_IDENTIFIER) {
            tokenInfo.opPrototypes[tokenInfo.mkOpCode(opText)] = p;
          } else {
            tokenInfo.opPrototypes[opCode] = p;
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

          if (itemList.size() > 3 and mode != MODE_BLOCK_COMMENT) {

            // shift list by two
            std::vector<Token const*> subItemList(++ ++itemList.begin(), itemList.end());

            // parse sub mode field
            ParserMode subMode = parseModeField(subItemList[1]);
            Log(LS::DEBUG|LS::CONFIG, "config: paren sub mode field:  " << subMode << "\n");

            // parse additional mode, precedence fields...
            ParserAssoc assoc = subMode == MODE_BINARY ? ASSOC_LEFT : ASSOC_NONE;
            OutputMode outputMode = OUTPUT_MODE_NESTED;
            int precedence = 100;
            int unary_precedence = 100;
            bool ignoreIfStray = false;

            parseOperOptions(subItemList, assoc, outputMode, ignoreIfStray, precedence, unary_precedence);

            TokenProto *tp = tokenInfo.getProto(&token);

            tp->mode = subMode;
            tp->precedence = precedence;
            tp->outputMode = outputMode;
            tp->associativity = assoc;
            tp->ignoreIfStray = ignoreIfStray;

            for (auto it = tp->endList.begin(); it != tp->endList.end(); ++it) {
              TokenProto *etp = tokenInfo.getProto(&it->second);
              etp->mode = subMode;
              etp->precedence = precedence;
              etp->outputMode = outputMode;
              etp->associativity = assoc;
              etp->ignoreIfStray = ignoreIfStray;
            }

          } else if (mode == MODE_BLOCK_COMMENT) {
            TokenProto *tp = tokenInfo.getProto(&token);
            tp->mode = MODE_BLOCK_COMMENT;
            assert(tp->isParen);
            for (auto it = tp->endList.begin(); it != tp->endList.end(); ++it) {
              TokenProto *etp = tokenInfo.getProto(&it->second);
              etp->mode = MODE_BLOCK_COMMENT;
              assert(etp->isRParen);
            }
          }
        }

      }
    } else {
      if (defHead) {
        Log(LS::ERROR|LS::CONFIG, "config: error: invalid line " << defHead->line << "\n");
      } else {
        Log(LS::DEBUG|LS::CONFIG, "config: empty line " << line->line << "\n");
      }
    }
  }
  return true;
}

bool debugInit = false;

void printCopyright() {
  printf("%s\n", getCopyright().c_str());
}

void printBugreportInfo() {
#ifdef PACKAGE_URL
  printf("Visit us on the web at %s\n", PACKAGE_URL);
#endif
  printf("Report bugs to %s\n", PACKAGE_BUGREPORT);
}

void printVersion() {
  cmdline_parser_print_version();
  printf("Version tag: %s\n", std::string(vcs_version).substr(0,8).c_str());
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
  // TODO: add help...
  // gennc_parsertoken_full_help_string();
  // gennc_logger_full_help_string();
  // gennc_scanner_full_help_string();
  // gennc_assoc_full_help_string();
  // gennc_modes_full_help_string();
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

void cleanup_cmdline_parser() {
  cmdline_parser_free(&args);
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

  atexit(cleanup_cmdline_parser);
  cmdline_parser_init(&args);

  std::string configFileName = upUserDir + "/" PACKAGE_NAME "-options";
  if (debugInit) {
    std::cerr << "P2X user config file: " << configFileName << "\n";
  }

  int rc = readConfigFile(configFileName);
  if (rc == 1) {
    Log(LS::ERROR, "invalid configuration file " << configFileName << "\n");
    exit(EXIT_FAILURE);
  }

  {
    cmdline_parser_params cmdlineParams;
    cmdline_parser_params_init(&cmdlineParams);
    cmdlineParams.initialize = 0;
    cmdlineParams.override = 1;
    cmdlineParams.check_required = 0;
    if (cmdline_parser_ext(argc, argv, &args, &cmdlineParams)!=0) {
      Log(LS::ERROR, "invalid command line parameters\n");
      exit(EXIT_FAILURE);
    }
  }

  if (cmdline_parser_required(&args, argv[0]) != 0) {
    Log(LS::ERROR, "invalid configuration\n");
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
            Log(LS::DEBUG, "Parsed arg of " << i << "-th -v flag as integer: " << code << "\n");
            vLevel |= code;
          } else {
            int ok;
            LS::LogLevel code = LS::getLogLevelValue(args.verbose_arg[i], &ok);
            if (ok == 0) {
              Log(LS::DEBUG, "Parsed arg of " << i << "-th -v flag as level name: " << code << "\n");
              vLevel |= code;
            } else {
              Log(LS::ERROR, "Arg of " << i << "-th -v flag is not an integer"
                  " and not a valid log level name: " << args.verbose_arg[i] << "\n");
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
    Log(LS::INFO, "Setting verbosity level to " << std::hex << "0x" << LS::mask << std::dec << "\n");
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

  options.code = args.attribute_code_flag;

  options.newlineAsBr = args.newline_as_br_flag;
  options.newlineAsEntity = args.newline_as_entity_flag;
  options.indent = args.indent_flag;
  options.merged = args.merged_flag;
  options.strict = args.strict_flag;
  options.loose  = args.loose_flag;
  options.sparse = args.sparse_flag;
  options.writeRec = args.write_recursive_flag;

  if (args.null_given) {
    options.nullName = args.null_arg;
  }

  std::string indentUnit = args.indent_unit_arg[0];
  if (args.indent_unit_given) {
    indentUnit = args.indent_unit_arg[args.indent_unit_given -1];
  }

  bool xmlDeclDefault = false;
  if (args.input_encoding_given>0) {
    options.encoding = args.input_encoding_arg[args.input_encoding_given -1];
    xmlDeclDefault = true;
  } else {
    options.encoding = args.input_encoding_arg[0];
  }

  if (args.write_xml_declaration_given) {
    options.xmlDecl = args.write_xml_declaration_flag;
  } else {
    options.xmlDecl = xmlDeclDefault;
  }

  if (args.include_config_flag || args.element_scanner_flag) {
    options.scanConf = true;
  }
  if (args.include_config_flag || args.element_parser_flag) {
    options.parseConf = true;
  }
  if (args.include_config_flag || args.element_treewriter_flag) {
    options.treewriterConf = true;
  }
  if (args.element_ca_steps_flag) {
    options.caSteps = true;
  }
  // if (args.element_ca_version_given) {
    // options.caVersion = args.element_ca_version_flag;
  // }
  // if (args.comment_version_given) {
    // options.commentVersion = args.comment_version_flag;
  // }
  options.caVersion = true;
  options.commentVersion = true;
  options.bom = args.write_bom_flag;

  std::ostream *_out = &std::cout;

  if (args.outfile_given) {
    _out = new std::ofstream(args.outfile_arg, std::ios::binary);
    Log(LS::FILES, "Open file " << args.outfile_arg << " for output\n");
    if (!*_out) {
      Log(LS::ERROR, "Failed to open output file: " << strerror(errno) << "\n");
      return 1;
    }
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
      Log(LS::ERROR,  "error: Invalid scanner name '" << scannerTypeName << "', using default scanner "
          << getScannerTypeName(scannerType) << "\n")
    }
  }

  Log(LS::DEBUG|LS::CONFIG, "Number of input files: " << args.inputs_num << std::endl);
  std::vector<std::string> fileList;
  for (unsigned i = 0 ; i < args.inputs_num; ++i ) {
    Log(LS::DEBUG|LS::CONFIG, "Input file " << i << ": " << args.inputs[i] << std::endl);
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
        Log(LS::ERROR, "Failed to open config file " << precPath << ": " << strerror(errno) << "\n");
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
        Log(LS::FILES|LS::INFO|LS::DEBUG, "Parsing config file " << precPath << "\n");
        Token *croot = configParser.parseStream(configFile);
        TreeXMLWriter::Options options;
        options.line = options.col = options.prec /* = options.sparse */ = 1;
        TreeXMLWriter treeXMLWriter(configParser.tokenInfo, options);
        // LS::mask |= LS::DEBUG;
        Log(LS::DEBUG|LS::CONFIG, "Dumping XML of config file\n");
        IfLog(LS::DEBUG|LS::CONFIG, treeXMLWriter.writeXML(croot, getls(), ""););
        Lexer lexer(scannerType);
        parseConfig(lexer, precPath, croot, tokenInfo);
        Log(LS::IO, "Read " << configFile.tellg() << " B from config file '" << precPath << "'\n");
        // LS::mask &= ~LS::DEBUG;
      } else {
        Log(LS::ERROR, "Failed to read config file " << precPath << ": " << strerror(errno) << "\n");
        exit(3);
      }
    }
  }

  int precedence = 10;

  struct OpInfo {
    ParserToken token;
    std::string name;
    int precedence;
    bool isToken() const {
      return token != TOKEN_EOF and token != TOKEN_ROOT;
    }
  };

  auto isToken = [](OpInfo const& name) -> bool {
    return name.isToken();
  };

  auto getarg = [&precedence](std::string name) -> OpInfo {
    size_t eqind = name.find('=');
    int prec = precedence;
    if (eqind != name.npos) {
      std::istringstream str(name.substr(eqind+1));
      str >> prec;
      name = name.substr(0, eqind);
    }
    int ok;
    ParserToken tok = Token::getParserTokenValue(name, &ok);
    OpInfo info = {tok, name, prec};
    if (!info.isToken()) {
      info.token = TOKEN_EOF;
      if ((name[0] == '\'' and name[name.size()-1] == '\'')
          or (name[0] == '"' and name[name.size()-1] == '"')) {
        info.name = name.substr(1, name.size()-2);
      }
    }
    return info;
  };

  if (args.binary_given>0) {
    for (unsigned i = 0; i < args.binary_given; ++i) {
      std::string name = args.binary_arg[i];
      auto args = getarg(name);
      if (args.token != TOKEN_EOF) {
        tokenInfo.addBinary(args.token, args.precedence);
      } else {
        tokenInfo.addBinary(args.name, args.precedence);
      }
      ++precedence;
    }
  }

  if (args.right_given>0) {
    precedence = 1000;
    for (unsigned i = 0; i < args.right_given; ++i) {
      std::string name = args.right_arg[i];
      auto args = getarg(name);
      if (args.token != TOKEN_EOF) {
        tokenInfo.addBinary(args.token, args.precedence, ASSOC_RIGHT);
      } else {
        tokenInfo.addBinary(args.name, args.precedence, ASSOC_RIGHT);
      }
      ++precedence;
    }
  }

  if (args.unary_given>0) {
    precedence = 2000;
    for (unsigned i = 0; i < args.unary_given; ++i) {
      std::string name = args.unary_arg[i];
      auto args = getarg(name);
      if (args.token != TOKEN_EOF) {
        tokenInfo.addUnary(args.token, args.precedence);
      } else {
        tokenInfo.addUnary(args.name, args.precedence);
      }
      ++precedence;
    }
  }

  if (args.postfix_given>0) {
    precedence = 3000;
    for (unsigned i = 0; i < args.postfix_given; ++i) {
      std::string name = args.postfix_arg[i];
      auto args = getarg(name);
      if (args.token != TOKEN_EOF) {
        tokenInfo.addPostfix(args.token, args.precedence);
      } else {
        tokenInfo.addPostfix(args.name, args.precedence);
      }
      ++precedence;
    }
  }

  if (args.ignore_given>0) {
    for (unsigned i = 0; i < args.ignore_given; ++i) {
      std::string name = args.ignore_arg[i];
      auto args = getarg(name);
      if (args.token != TOKEN_EOF) {
        tokenInfo.addIgnore(args.token);
      } else {
        tokenInfo.addIgnore(args.name);
      }
    }
  }

  if (args.item_given>0) {
    for (unsigned i = 0; i < args.item_given; ++i) {
      std::string name = args.item_arg[i];
      auto args = getarg(name);
      if (args.token != TOKEN_EOF) {
        tokenInfo.addItem(args.token);
      } else {
        tokenInfo.addItem(args.name);
      }
    }
  }

  if (args.brace_given>0) {
    for (unsigned i = 0; i < args.brace_given; ++i) {
      std::string astr = args.brace_arg[i];
      size_t pc = astr.find_first_of(",;:");
      if (pc == std::string::npos) {
        Log(LS::ERROR|LS::PARSE, "Parser: error: command line brace definition must be of the form start:end\n");
        exit(4);
      }
      std::string name1 = astr.substr(0, pc);
      std::string name2 = astr.substr(pc+1);
      auto args1 = getarg(name1);
      auto args2 = getarg(name2);
      if (isToken(args1) and isToken(args2)) {
        tokenInfo.addBrace(args1.token, args2.token);
      } else if (isToken(args1)) {
        tokenInfo.addBrace(args1.token, args2.name);
      } else if (isToken(args2)) {
        tokenInfo.addBrace(args1.name, args2.token);
      } else {
        tokenInfo.addBrace(args1.name, args2.name);
      }
    }
  }

  if (args.list_classes_given) {
    TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
    if (treeXMLWriter.options.bom)
      out << char(0xff);
    if (treeXMLWriter.options.xmlDecl)
      out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
    out << "<parser xmlns='" NAMESPACE_CX "' xmlns:ca='" NAMESPACE_CA "'>\n";
    treeXMLWriter.writeXML(tokenInfo, out, indentUnit);
    out << "</parser>\n";
    return 0;
  }


  // Start processing input
  std::istream *inFile = &std::cin, *infile = 0;
  if (not fileList.empty()) {
    infile = new std::ifstream(fileList[0].c_str());
    if (infile and *infile) {
      Log(LS::FILES, "Open file " << fileList[0] << "\n");
      inFile = infile;
    } else {
      Log(LS::ERROR, "Failed to open input file: " << fileList[0] << ": " << strerror(errno) << std::endl);
      delete infile;
      if (_out != &std::cout) delete _out;
      return EXIT_FAILURE;
    }
  } else {
    if (isatty(0) and not args.stdin_tty_given) {
      Log(LS::ERROR, "Refusing to read from the keyboard, use --stdin-tty to overrule" << std::endl);
      if (_out != &std::cout) delete _out;
      return EXIT_FAILURE;
    }
  }

  std::stringstream inStream_, *inStream = &inStream_;
  *inStream << inFile->rdbuf();

  Log(LS::IO, "Read " << inStream->tellp() << " B from file "
      << (fileList.size() ? fileList[0].c_str() : "input")
      << "\n");

  if (args.scan_only_given) {
    Timer tScanner;
    Log(LS::TIMES, "Scanning input... ");
    Scanner scanner(scannerType);
    scanner.readTokenList(*inStream);
    Log(LS::TIMES, "done in " << tScanner << " s" << std::endl);
    Timer tScanXML;
    Log(LS::TIMES, "Writing scan list to XML... ");
    TreeXMLWriter treeXMLWriter(tokenInfo, options, indentUnit);
    if (treeXMLWriter.options.bom)
      out << char(0xff);
    if (treeXMLWriter.options.xmlDecl)
      out << "<?xml version=\"1.0\" encoding=\"" << treeXMLWriter.options.encoding << "\"?>\n";
    out << "<scan-xml xmlns='" NAMESPACE_CX "' xmlns:ca='" NAMESPACE_CA "'>\n";
    if (options.caVersion) {
      writeVersionInfoXML(options, indentUnit, out);
      out << "\n";
    }
    out << indentUnit << "<ca:scanner type='" << scannerType << "'/>" << "\n";
    treeXMLWriter.writeXML(scanner.tokenList, out, indentUnit);
    out << "</scan-xml>\n";
    Log(LS::TIMES, "done in " << tScanner << " s" << std::endl);
    out.flush();
    if (!out) {
      Log(LS::ERROR, "Stream failure while writing Token list: " << strerror(errno) << "\n");
      return 1;
    }
    if (infile) {
      delete infile;
      infile = 0;
    }
    if (_out != &std::cout) {
      delete _out;
      _out = 0;
    }
    return 0;
  }


  FPParser fpParser(scannerType);
  fpParser.options.inputFileName = (fileList.size() ? fileList[0].c_str() : "stdin");
  if (args.noignore_given) {
    fpParser.options.ignoreIgnore = true;
  }
  fpParser.tokenInfo = tokenInfo;

  Token::count = 0;

  Token *root = fpParser.parseStream(*inStream);

  Timer tXML;
  Log(LS::TIMES, "Writing tree... ");

  std::string outputMode = "x";
  if (args.matlab_given) {
    outputMode = "m";
  }
  if (args.json_given) {
    outputMode = "j";
  }
  if (args.xml_given) {
    outputMode = "y";
  }
  if (args.output_mode_given) {
    outputMode = args.output_mode_arg[0];
  }

  if (outputMode == "x")
    writeTreeXML(root, tokenInfo, options, indentUnit, out, fpParser.scanner.m_type);
  else if (outputMode == "y") {
    options.type = true;
    options.prefix_ca = "c";
    options.prefix_ci = "i";
    options.textTagName = "t";
    writeTreeXML2(root, tokenInfo, options, indentUnit, out, scannerType);
  } else if (outputMode == "j")
    writeTreeJSON(root, tokenInfo, options, indentUnit, out, fpParser.scanner.m_type);
  else if (outputMode == "m")
    writeTreeMATLAB(root, tokenInfo, options, indentUnit, out, fpParser.scanner.m_type);
  else if (outputMode == "n")
    writeTreeMATLAB_LR(root, tokenInfo, options, indentUnit, out, fpParser.scanner.m_type);

  Log(LS::TIMES, "done in " << tXML << " s" << std::endl);
  Log(LS::IO, "Wrote " << out.tellp() << " B to file '" << args.outfile_arg << "'\n");

  out.flush();
  if (!out) {
    Log(LS::ERROR, "Stream failure while writing output: " << strerror(errno) << "\n");
    return 1;
  }

  if (infile) {
    delete infile;
    infile = 0;
  }
  if (_out != &std::cout) {
    delete _out;
    _out = 0;
  }

}
#endif

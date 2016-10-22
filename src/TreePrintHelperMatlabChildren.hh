#pragma once

struct TreePrintHelperMATLABChildren : public TreePrintHelperMATLABLR {
  std::string itemStart, itemEnd, childStart, childEnd, nvsep, comma, space1, stmtSep, linebreak, emptyList, codebreak, namequote;
  std::ostream* textout;
  bool stringconcat;

  TreePrintHelperMATLABChildren(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    TreePrintHelperMATLABLR(xmlWriter, aus)
  {
    itemStart = "struct(";
    itemEnd = ")";
    childStart = "{";
    childEnd = "}";
    nvsep = ",";
    comma = ",";
    space1 = " ";
    stmtSep = ";";
    linebreak = "\n";
    emptyList = "[]";
    codebreak = "...";
    namequote = "\'";
    textout = &maus;
    stringconcat = true;
  }

  struct TermCollector {

    TreePrintHelperMATLABChildren const &m_cont;
    std::function<void(Token const *t)> m_fcn;

    TermCollector(TreePrintHelperMATLABChildren const &m_cont, std::function<void(Token const *t)> fcn) :
      m_cont(m_cont),
      m_fcn(fcn)
    {
    }

    int onEnter(Token const *t, Token const *) {
      bool merged = m_cont.m_xmlWriter.options.merged
        or m_cont.m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;

      int res = 0;
      if (not t) return res;

      if (not (t->left and TokenTypeEqual(m_cont.m_xmlWriter.tokenInfo)(t, t->left) and merged)) {
        res |= (1 << 0);
        m_fcn(t->left);
      }

      return res;
    }
    int onContent(Token const *t, Token const *) {
      bool merged = m_cont.m_xmlWriter.options.merged
        or m_cont.m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;

      int res = 0;
      if (not t) return res;

      if (not (t->right and TokenTypeEqual(m_cont.m_xmlWriter.tokenInfo)(t, t->right) and merged)) {
        res |= (1 << 1);
        m_fcn(t->right);
      }

      return res;
    }
    int onLeave(Token const *, Token const *) { return 0; }

  };

  virtual void collectTerms__(Token const *t, std::function<void(Token const *t)> fcn) {
    TermCollector termCollector (*this, fcn);
    traverseTree(termCollector, t);
  }

  bool findAny(Token const *t, std::function<bool (Token const *t)> fcn) {
    if (t == 0) return false;
    if (fcn(t)) return true;
    if (findAny(t->left, fcn)) return true;
    if (findAny(t->right, fcn)) return true;
    return false;
  }

  template<class IT>
  bool findAny(IT start, IT const &end, std::function<bool (typename IT::value_type)> fcn) {
    if (start == end) return false;
    if (fcn(*start)) return true;
    return findAny(++start, end, fcn);
  }

  template<class ValueType>
  bool check(ValueType const &) { return true; }

  bool check(std::string const &t) { return t.find_first_of("\n\r") != std::string::npos; }

  template<class ValueType>
  void collectTerms(Token const *t, std::string const &name, std::function<ValueType (Token const *t)> fcn, bool quote = false) {
    std::list<Token const *> sterms;
    collectTerms__(t, [&](Token const *t) -> void { sterms.push_back(t); });
    aus << namequote << name << namequote << nvsep << childStart;
    for (auto it = sterms.begin(); it != sterms.end(); ++it) {
      ValueType val = fcn(*it);
      if (it != sterms.begin())
        aus << comma;
      if (quote) {
        bool const anySpecial = check(val);
        if (anySpecial and stringconcat) {
          aus << "[";
        }
        aus << namequote;
        if (*it) {
          *textout << val;
        }
        aus << namequote;
        if (anySpecial and stringconcat) {
          aus << "]";
        }
      } else
        aus << val;
    }
    aus << childEnd << comma << space1;
  }

  std::string getIgnore(Token const *t) {
    std::string res;
    if (t->ignore) {
      Token const *ignore = t->ignore;
      while (ignore) {
        res += ignore->text;
        ignore = ignore->ignore;
      }
    }
    return res;
  }

  virtual std::string getText(Token const *t) {
    return t->text;
  }

  virtual int onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onEnter " << (void*)t << " " << *t << "\n";
#endif
    setupNode(t);
    setIndent();

    tags = not(parent
               and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
               and merged);
    if (t->token == TOKEN_ROOT) {
      aus << itemStart << namequote << "n" << namequote << nvsep << namequote << elemName(t) << namequote;
      if (m_xmlWriter.options.id)
        aus << comma << namequote << "id" << namequote << nvsep << t->id;
      writeSFLocAttrs(t);

      aus << comma << namequote << "i" << namequote << nvsep;
      aus << namequote;
      if (t->ignore) {
        Token *ignore = t->ignore;
        while (ignore) {
          *textout << ignore->text;
          ignore = ignore->ignore;
        }
      }
      aus << namequote;

      aus << comma << namequote << "c" << namequote << nvsep << childStart;

    }

    if (tags and (t->left or t->right)) {
      if (m_xmlWriter.options.indent) {
        aus << codebreak << linebreak << indent;
      }

      aus << itemStart;
      collectTerms<std::string>(t, "n", [&](Token const *t) -> std::string { return t ? elemName(t) : ""; }, true);
      collectTerms<std::string>(t, "t", [&](Token const *t) -> std::string { return t ? getText(t) : namequote+namequote; }, true);
      collectTerms<std::string>(t, "i", [&](Token const *t) -> std::string { return t ? getIgnore(t) : ""; }, true);
      if (m_xmlWriter.options.line)
        collectTerms<int>(t, "ln", [&](Token const *t) -> int { return (t ? t->line : 0); });
      if (m_xmlWriter.options.col)
        collectTerms<int>(t, "cl", [&](Token const *t) -> int { return (t ? t->column : 0); });

      aus << namequote << "c" << namequote << nvsep << childStart;
      ++m_level;

    }

    if (t->left == 0) {
      aus << emptyList;
    }

    return 0;
  }
  virtual int onContent(Token const * t, Token const * parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif
    tags = not(parent
               and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
               and merged);
    if (not tags or t->left != 0 or t->right != 0) {
        aus << ",";
    }
    return 0;
  }
  virtual int onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

    setupNode(t);

    tags = not(parent
               and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
               and merged);
    if ((not tags or t->left) and t->right == 0) {
      aus << emptyList;
    }
    if (tags and (t->left or t->right)) {
      aus << childEnd << itemEnd;
      --m_level;
    }
    if (t->token == TOKEN_ROOT) {
      aus << childEnd << itemEnd << stmtSep << linebreak;
    }
    return 0;
  }

};


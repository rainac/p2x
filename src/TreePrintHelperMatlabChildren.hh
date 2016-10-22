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
    space1 = xmlWriter.options.indent ? " " : "";
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

  virtual void collectTerms(Token const *t, std::function<void(Token const *t)> fcn) {
    TermCollector termCollector(*this, fcn);
    traverseTree(termCollector, t);
  }

  virtual void collectChildren(Token const *t, std::list<Token const *> &sterms) {
    collectTerms(t, [&](Token const *t) -> void { sterms.push_back(t); });
  }

  template<class ValueType>
  bool check(ValueType const &) { return false; }
  bool check(std::string const &t) { return t.find_first_of("\n\r") != std::string::npos; }

  template<class ValueType>
  void printTerms(Token const *, std::list<Token const*> const &sterms,
                  std::string const &name, std::function<ValueType (Token const *t)> fcn, bool quote = false) {
    std::vector<ValueType> vterms;
    for (auto it = sterms.begin(); it != sterms.end(); ++it) {
      ValueType val = fcn(*it);
      vterms.push_back(val);
    }
    std::vector<ValueType> vterms2 = vterms;
    auto nend = std::unique(vterms2.begin(), vterms2.end());
    std::function<void(ValueType const &v)> fprint = [&](ValueType const &val) -> void {
      if (quote) {
        bool const anySpecial = check(val);
        if (anySpecial and stringconcat) {
          aus << "[";
        }
        aus << namequote;
        *textout << val;
        aus << namequote;
        if (anySpecial and stringconcat) {
          aus << "]";
        }
      } else
        aus << val;
    };
    if (nend == vterms2.begin()+1) {
      nend = vterms2.begin();
      if (*nend == ValueType()) {
      } else {
        aus << namequote << name << namequote << nvsep;
        ValueType const &val = *nend;
        fprint(val);
        aus << comma << space1;
      }
    } else {
      aus << namequote << name << namequote << nvsep << childStart;
      for (auto it = vterms.begin(); it != vterms.end(); ++it) {
        ValueType const &val = *it;
        if (it != vterms.begin())
          aus << comma;
        fprint(val);
      }
      aus << childEnd << comma << space1;
    }
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

      std::list<Token const *> sterms;
      collectChildren(t, sterms);

      aus << itemStart;
      printTerms<std::string>(t, sterms, "n", [&](Token const *t) -> std::string { return t ? elemName(t) : ""; }, true);
      printTerms<std::string>(t, sterms, "t", [&](Token const *t) -> std::string { return t ? getText(t) : ""; }, true);
      printTerms<std::string>(t, sterms, "i", [&](Token const *t) -> std::string { return t ? getIgnore(t) : ""; }, true);
      if (m_xmlWriter.options.line)
        printTerms<int>(t, sterms, "ln", [&](Token const *t) -> int { return (t ? t->line : 0); });
      if (m_xmlWriter.options.col)
        printTerms<int>(t, sterms, "cl", [&](Token const *t) -> int { return (t ? t->column : 0); });

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


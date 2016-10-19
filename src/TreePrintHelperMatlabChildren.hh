#pragma once

struct TreePrintHelperMATLABChildren : public TreePrintHelperMATLABLR {

  TreePrintHelperMATLABChildren(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    TreePrintHelperMATLABLR(xmlWriter, aus)
  {}

  virtual void collectTerms_(Token const *t, std::function<void(Token const *t)> fcn) {
    bool merged = m_xmlWriter.options.merged
      or m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;

    if (not t) return;

    if (t->left and TokenTypeEqual(m_xmlWriter.tokenInfo)(t, t->left) and merged) {
      collectTerms_(t->left, fcn);
    } else {
      fcn(t->left);
    }

    if (t->right and TokenTypeEqual(m_xmlWriter.tokenInfo)(t, t->right) and merged) {
      collectTerms_(t->right, fcn);
    } else {
      fcn(t->right);
    }

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
    collectTerms_(t, [&](Token const *t) -> void { sterms.push_back(t); });
    aus << "'" << name << "',{";
    for (auto it = sterms.begin(); it != sterms.end(); ++it) {
      ValueType val = fcn(*it);
      if (it != sterms.begin())
        aus << ",";
      if (quote) {
        bool const anySpecial = check(val);
        if (anySpecial) {
          aus << "[";
        }
        aus << "'";
        if (*it) {
          maus << val;
        }
        aus << "'";
        if (anySpecial) {
          aus << "]";
        }
      } else
        aus << val;
    }
    aus << "}, ";
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

  virtual void onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onEnter " << (void*)t << " " << *t << "\n";
#endif
    if (t->left or t->right) {
      ++m_level;
    }
    setupNode(t);
    setIndent();

    tags = not(parent
               and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
               and merged);
    if (t->token == TOKEN_ROOT) {
      aus << "struct('n','" << elemName(t) << "'";
      if (m_xmlWriter.options.id)
        aus << ",'id'," << t->id << "";
      writeSFLocAttrs(t);

      if (t->token == TOKEN_NEWLINE) {
        aus << ",'t',";
        aus << "char(10)";
      } else if (t->token == TOKEN_CRETURN) {
        aus << ",'t',";
        aus << "char(13)";
      } else if (not t->text.empty()) {
        aus << ",'t','";
        maus << t->text;
        aus << "'" << "";
      }

      aus << ",'i',";
      aus << "'";
      if (t->ignore) {
        Token *ignore = t->ignore;
        while (ignore) {
          maus << ignore->text;
          ignore = ignore->ignore;
        }
      }
      aus << "'";

      aus << ",";
      aus << "'c',";

    }

    if (tags and (t->left or t->right)) {
      if (m_xmlWriter.options.indent) {
        aus << "...\n" << indent;
      }

      aus << "struct(";
      collectTerms<std::string>(t, "n", [&](Token const *t) -> std::string { return t ? elemName(t) : ""; }, true);
      collectTerms<std::string>(t, "t", [&](Token const *t) -> std::string { return t ? getText(t) : "''"; }, true);
      collectTerms<std::string>(t, "i", [&](Token const *t) -> std::string { return t ? getIgnore(t) : ""; }, true);
      if (m_xmlWriter.options.line)
        collectTerms<int>(t, "ln", [&](Token const *t) -> int { return (t ? t->line : 0); });
      if (m_xmlWriter.options.col)
        collectTerms<int>(t, "cl", [&](Token const *t) -> int { return (t ? t->column : 0); });

      aus << "'c',{";

    }

    if (t->left == 0) {
      aus << "[]";
    }

  }
  virtual void onContent(Token const * t, Token const * parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif
    tags = not(parent
               and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
               and merged);
    if (not tags or t->left != 0 or t->right != 0) {
        aus << ",";
    }
  }
  virtual void onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

    setupNode(t);

    tags = not(parent
               and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
               and merged);
    if (not tags and t->right == 0) {
      aus << "[]";
    }
    if (tags and (t->left or t->right)) {
      aus << "})";
    }
    if (t->token == TOKEN_ROOT) {
      aus << ");\n";
    }
    if (t->left or t->right) {
      --m_level;
    }
  }

};


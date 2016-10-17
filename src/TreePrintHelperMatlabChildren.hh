#pragma once

struct TreePrintHelperMATLABChildren : public TreePrintHelperMATLABLR {

  TreePrintHelperMATLABChildren(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    TreePrintHelperMATLABLR(xmlWriter, aus)
  {}

  virtual void collectTerms_(Token const *t, std::function<void(Token const *t)> fcn) {
    bool merged = m_xmlWriter.options.merged
      or m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;
    fcn(t->left);
    if (t->left) {
      if (TokenTypeEqual(m_xmlWriter.tokenInfo)(t, t->left) and merged) {
        collectTerms_(t->left, fcn);
      }
    }
    fcn(t->right);
    if (t->right) {
      if (TokenTypeEqual(m_xmlWriter.tokenInfo)(t, t->right) and merged) {
        collectTerms_(t->right, fcn);
      }
    }
  }

  template<class ValueType>
  void collectTerms(Token const *t, std::string const &name, std::function<ValueType (Token const *t)> fcn, bool quote = false) {
    std::list<ValueType> sterms;
    collectTerms_(t, [&](Token const *t) -> void { sterms.push_back(fcn(t)); });
    aus << "'" << name << "',{";
    for (auto it = sterms.begin(); it != sterms.end(); ++it) {
      if (it != sterms.begin())
        aus << ",";
      if (quote)
        aus << "'";
      if (quote)
        maus << *it;
      else
        aus << *it;
      if (quote)
        aus << "'";
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
    if (t->token == TOKEN_NEWLINE) {
      return "char(10)";
    } else {
      return "'" + t->text + "'";
    }
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

    if (t->left or t->right) {
      if (m_xmlWriter.options.indent) {
        aus << "...\n" << indent;
      }

      aus << "struct(";
      collectTerms<std::string>(t, "n", [&](Token const *t) -> std::string { return t ? elemName(t) : ""; }, true);
      collectTerms<std::string>(t, "t", [&](Token const *t) -> std::string { return t ? getText(t) : "''"; }, false);
      collectTerms<std::string>(t, "i", [&](Token const *t) -> std::string { return t ? getIgnore(t) : ""; }, true);
      collectTerms<int>(t, "ln", [&](Token const *t) -> int { return (t ? t->line : 0); });
      collectTerms<int>(t, "cl", [&](Token const *t) -> int { return (t ? t->column : 0); });

      aus << "'c',{";

    }

    if (t->left == 0) {
      aus << "[]";
    }

  }
  virtual void onContent(Token const * t, Token const * /* parent */) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif
    if (t->left != 0 or t->right != 0) {
        aus << ",";
    }
  }
  virtual void onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

    setupNode(t);

    tags = true || not(parent
                       and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                       and merged);
    if (t->left or t->right) {
      if (t->right == 0) {
        aus << "[]";
      }
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


#pragma once

struct TreePrintHelperMATLABMerged : public TreePrintHelperMATLAB {

  TreePrintHelperMATLABMerged(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    TreePrintHelperMATLAB(xmlWriter, aus)
  {}

  virtual void onEnter(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onEnter " << (void*)t << " " << *t << "\n";
#endif
    if (t->left or t->right) {
      ++m_level;
    }
    setupNode(t);
    setElemName(t);
    setIndent();

    tags = true || not(parent
                       and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                       and merged);
    if (tags) {
      aus << "struct('n','" << elemName << "'";
      if (m_xmlWriter.options.id)
        aus << ",'id'," << t->id << "";
      writeSFLocAttrs(t);
    }

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

    if (t->ignore) {
      aus << ",'i',";
      aus << "'";
      Token *ignore = t->ignore;
      while (ignore) {
        maus << ignore->text;
        ignore = ignore->ignore;
      }
      aus << "'";
    }

    if (t->left or t->right) {
      aus << ",";
      if (m_xmlWriter.options.indent) {
        aus << "...\n" << indent;
      }
      aus << "'c',[";
    }
  }
  virtual void onContent(Token const *t, Token const * /* parent */) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif

    setupNode(t);
    setIndent();

    if (t->left and t->right) {
      aus << ",";
    }
  }
  virtual void onLeave(Token const *t, Token const *parent) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onLeave " << (void*)t << " " << *t << "\n";
#endif

    setupNode(t);
    setElemName(t);

    tags = true || not(parent
                       and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                       and merged);
    if (tags) {
      setIndent();
      if (t->left or t->right) {
        aus << "]";
      }
      aus << ")";
    }
    if (t->token == TOKEN_ROOT) {
      aus << ";\n";
    }
    if (t->left or t->right) {
      --m_level;
    }
  }

};


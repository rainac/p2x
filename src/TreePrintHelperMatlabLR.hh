#pragma once

struct TreePrintHelperMATLAB {
  TreeXMLWriter const &m_xmlWriter;
  size_t m_level;
  std::string indent, subindent;
  bool merged, tags;
  std::ostream &aus;
  OstreamMATLABEscape maus;

  TreePrintHelperMATLAB(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    m_xmlWriter(xmlWriter),
    m_level(),
    aus(aus),
    maus(aus)
  {}

  virtual ~TreePrintHelperMATLAB() {}

  virtual void setWhiteLen(std::string &str, size_t ilevel) const {
    if (str.size() < ilevel) {
      str.insert(str.end(), ilevel-str.size(), m_xmlWriter.indentUnit[0]);
    }
    if (str.size() > ilevel) {
      str.resize(ilevel);
    }
  }

  virtual void setIndent() {
    if (m_xmlWriter.options.indent) {
      int ilevel = std::min<int>(m_level, m_xmlWriter.options.minStraightIndentLevel + log(std::max<size_t>(m_level, 1)));
#ifndef NDEBUG
      ls(LS::DEBUG|LS::PARSE) << "rec. level -> indent level: " << m_level << " -> " << ilevel << "\n";
#endif
      setWhiteLen(indent, ilevel);
      setWhiteLen(subindent, ilevel+1);
    }
  }

  virtual std::string elemName(Token const *t) {
    std::string elemName;
    if (m_xmlWriter.tokenInfo.isParen(t)) {
      elemName = "par";
    } else if (t->token == TOKEN_STRING) {
      elemName = "str";
    } else if (t->token == TOKEN_FLOAT or t->token == TOKEN_INTEGER) {
      elemName = "num";
    } else if (t->token == TOKEN_IDENTIFIER) {
      if (m_xmlWriter.tokenInfo.mode(t) == MODE_ITEM) {
        elemName = "id";
      } else {
        elemName = "op";
      }
    } else if (t->token == TOKEN_ROOT) {
      elemName = "rt";
    } else {
      elemName = "op";
    }
    return elemName;
  }

  virtual void setupNode(Token const *t) {
    merged = m_xmlWriter.options.merged
      or m_xmlWriter.tokenInfo.outputMode(t) == OUTPUT_MODE_MERGED;
  }

  virtual void writeSFLocAttrs(Token const *t) const {
    if (m_xmlWriter.options.line)
      aus << ",'ln'," << t->line << "";
    if (m_xmlWriter.options.col)
      aus << ",'cl'," << t->column << "";
    if (m_xmlWriter.options._char)
      aus << ",'ch'," << t->character << "";
  }

  virtual void writeSFTypeAttrs(Token const *t) const {
    if (t->token == TOKEN_IDENTIFIER) {
      aus << ",'code'," << m_xmlWriter.tokenInfo.getOpCode(t) << "";
      aus << ",'repr','" << t->text << "'";
    } else {
      aus << ",'code'," << int(t->token) << "";
    }
    if (m_xmlWriter.options.type)
      aus << ",'type','" << Token::getParserTokenName(t->token) << "'";
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

    tags = true || not(parent
                       and TokenTypeEqual(m_xmlWriter.tokenInfo)(parent, t)
                       and merged);
    if (tags) {
      aus << "struct('n','" << elemName(t) << "'";
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

    if (t->left) {
      aus << ",";
      if (m_xmlWriter.options.indent) {
        aus << "...\n" << indent;
      }
      aus << "'l',";
    } else if (t->right and m_xmlWriter.options.strict) {
      aus << ",'l',''";
    }
  }
  virtual void onContent(Token const *t, Token const * /* parent */) {
#ifndef NDEBUG
    ls(LS::DEBUG|LS::PARSE) << "parse: onContent " << (void*)t << " " << *t << "\n";
#endif

    setupNode(t);
    setIndent();

    if (t->right) {
      aus << ",";
      if (m_xmlWriter.options.indent) {
        aus << "...\n" << indent;
      }
      aus << "'r',";
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
    if (tags) {
      setIndent();
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


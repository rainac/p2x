#pragma once

struct TreePrintHelperJSONChildren : public TreePrintHelperMATLABChildren {

  OstreamJSONEscape jaus;

  TreePrintHelperJSONChildren(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    TreePrintHelperMATLABChildren(xmlWriter, aus),
    jaus(aus)
  {
    itemStart = "{";
    itemEnd = "}";
    childStart = "[";
    childEnd = "]";
    nvsep = ":";
    // comma = ",";
    // space1 = " ";
    stmtSep = "";
    // linebreak = "\n";
    emptyList = "{}";
    codebreak = "";
    textout = &jaus;
    stringconcat = false;
    namequote = "\"";
  }

};


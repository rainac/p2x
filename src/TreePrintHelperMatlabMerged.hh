#pragma once

struct TreePrintHelperMATLABMerged : public TreePrintHelperMATLAB {

  TreePrintHelperMATLABMerged(TreeXMLWriter const &xmlWriter, std::ostream &aus) :
    TreePrintHelperMATLAB(xmlWriter, aus)
  {}

};


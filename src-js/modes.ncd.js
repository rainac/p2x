// -*- javascript -*- 
// This file has been automatically generated by 
// gennc.sh $Id: gennc-js.xsl 85 2016-01-14 21:29:43Z jwillkomm $
// from definition file ../src/xml/modes.ncd.xml.

if (typeof window == "undefined") {
  var ENUM = require('./gennc-common.js')
}
ENUM.createParserMode = function() {
 var k=0
 var prefix='MODE_'
 var index={
 IGNORE: (k=1),
 ITEM: ++k,
 UNARY: ++k,
 POSTFIX: ++k,
 BINARY: ++k,
 UNARY_BINARY: ++k,
 PAREN: (k=32),
 }
 var comments
 var res=ENUM.createENUM(index, prefix, comments)
 return res
}
ENUM.ParserMode=ENUM.createParserMode()
if (typeof window == 'undefined') {
 for (k in ENUM) {
  exports[k]=ENUM[k]
 }
}
  
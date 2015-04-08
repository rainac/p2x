// -*- javascript -*- 
// This file has been automatically generated by 
// gennc.sh $Id: gennc.xsl 31 2013-11-07 18:56:43Z jwillkomm $ 
// from definition file ../src/xml/assoc.ncd.xml.

    var ENUM = ENUM || {}
    ENUM.createParserAssoc = function() {
      var k = 0
      var prefix = 'ASSOC_'
      var index = {
        NONE: (k = 0),
        LEFT: ++k,
        RIGHT: ++k,
        
      }
      var findex = {
        ASSOC_NONE: (k = 0),
        ASSOC_LEFT: ++k,
        ASSOC_RIGHT: ++k,
        
      }
      var comments = {
        NONE: '',
        LEFT: '',
        RIGHT: '',
        
      }
      var names = []
      var names_index = {}
      for (n in index) {
        names.push(n)
        names_index[index[n]] = n
      }
      var names_l = names.map(function (n) { return n.toLowerCase(); })
      var res = findex
      res.prefix = prefix
      res.index = index
      res.full_index = findex
      res.names = names
      res.names_l = names_l
      res.names_index = names_index
      res.comments = comments
      return res
    }
    ENUM.ParserAssoc = ENUM.createParserAssoc()
    ENUM.getParserAssocName = function(code) {
      return ENUM.ParserAssoc.names_index[code]
    }
    ENUM.getParserAssocComment = function(code) {
      var name = ENUM.getParserAssocName(code)
      return ENUM.ParserAssoc.comments[name]
    }
    ENUM.getParserAssocValue = function(name) {
      if (name in ENUM.ParserAssoc.index) {
        return ENUM.ParserAssoc.index[name];
      } else {
         var pname = name.substring(ENUM.ParserAssoc.prefix.length)
         if (pname && pname in ENUM.ParserAssoc.index) {
            return ENUM.ParserAssoc.index[pname]
         } else if (ENUM.ParserAssoc.names_l.indexOf(name) > -1) {
           return ENUM.ParserAssoc.index[ENUM.ParserAssoc.names[ENUM.ParserAssoc.names_l.indexOf(name)]]
         } else if (ENUM.ParserAssoc.names_l.indexOf(pname) > -1) {
           return ENUM.ParserAssoc.index[ENUM.ParserAssoc.names[ENUM.ParserAssoc.names_l.indexOf(pname)]]
         }
      }
    }
    ENUM.getNumParserAssoc = function() {
      return ENUM.ParserAssoc.names.length
    }
    ENUM.getParserAssoc = function(which) {
       var name = ENUM.ParserAssoc.names[which]
       return ENUM.getParserAssocValue(name)
    }
    
    if (typeof window == 'undefined') {
       for (k in ENUM) {
          exports[k] = ENUM[k]
       }
       
    }
  
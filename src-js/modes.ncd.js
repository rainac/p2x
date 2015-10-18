// -*- javascript -*- 
// This file has been automatically generated by 
// gennc.sh $Id: gennc-js.xsl 69 2015-04-15 07:15:49Z jwillkomm $ 
// from definition file ../src/xml/modes.ncd.xml.

    var ENUM = ENUM || {}
    ENUM.createParserMode = function() {
      var k = 0
      var prefix = 'MODE_'
      var index = {
        IGNORE: (k = 1),
        ITEM: ++k,
        UNARY: ++k,
        POSTFIX: ++k,
        BINARY: ++k,
        UNARY_BINARY: ++k,
        PAREN: (k = 32),
        
      }
      k = 0
      var findex = {
        MODE_IGNORE: (k = 1),
        MODE_ITEM: ++k,
        MODE_UNARY: ++k,
        MODE_POSTFIX: ++k,
        MODE_BINARY: ++k,
        MODE_UNARY_BINARY: ++k,
        MODE_PAREN: (k = 32),
        
      }
      var comments = {
        IGNORE: '',
        ITEM: '',
        UNARY: '',
        POSTFIX: '',
        BINARY: '',
        UNARY_BINARY: '',
        PAREN: '',
        
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
    ENUM.ParserMode = ENUM.createParserMode()
    ENUM.getParserModeName = function(code) {
      return ENUM.ParserMode.names_index[code]
    }
    ENUM.getParserModeComment = function(code) {
      var name = ENUM.getParserModeName(code)
      return ENUM.ParserMode.comments[name]
    }
    ENUM.getParserModeValue = function(name) {
      if (name in ENUM.ParserMode.index) {
        return ENUM.ParserMode.index[name];
      } else {
         var pname = name.substring(ENUM.ParserMode.prefix.length)
         if (pname && pname in ENUM.ParserMode.index) {
            return ENUM.ParserMode.index[pname]
         } else if (ENUM.ParserMode.names_l.indexOf(name) > -1) {
           return ENUM.ParserMode.index[ENUM.ParserMode.names[ENUM.ParserMode.names_l.indexOf(name)]]
         } else if (ENUM.ParserMode.names_l.indexOf(pname) > -1) {
           return ENUM.ParserMode.index[ENUM.ParserMode.names[ENUM.ParserMode.names_l.indexOf(pname)]]
         }
      }
    }
    ENUM.getNumParserMode = function() {
      return ENUM.ParserMode.names.length
    }
    ENUM.getParserMode = function(which) {
       var name = ENUM.ParserMode.names[which]
       return ENUM.getParserModeValue(name)
    }
    
    if (typeof window == 'undefined') {
       for (k in ENUM) {
          exports[k] = ENUM[k]
       }
       
    }
  
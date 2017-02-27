// -*- javascript -*- 
// This file is part of Named Constant Generator.
// Copyright © 2016,2017 Johannes Willkomm
// See the file gennc for copying conditions.

var ENUM = ENUM || {}
ENUM.createENUM = function(index, prefix, comments) {
    var k, n, full_index = {}, symbols = {}
    for (k in index) {
        full_index[prefix + k] = index[k]
        symbols[prefix + k] = index[k]
    }
    var res = full_index
    res.symbols = symbols
    res.full_index = full_index
    res.index = index
    res.prefix = prefix
    res.comments = comments
    res.names_index = {}
    for (n in index) {
        res.names_index[index[n]] = n
    }
    res.names = Object.keys(index)
    res.names_l = res.names.map(function (n) { return n.toLowerCase(); })
    
    res.getName = function(code) {
        return this.names_index[code]
    }
    res.getComment = function(code) {
        var name = this.getName(code)
        return this.comments[name]
    }
  
    res.getValue = function(name) {
        if (name in this.index) {
            return this.index[name];
        } else {
            var pname = name.substring(this.prefix.length)
            if (pname && pname in this.index) {
                return this.index[pname]
            } else if (this.names_l.indexOf(name) > -1) {
                return this.index[this.names[this.names_l.indexOf(name)]]
            } else if (this.names_l.indexOf(pname) > -1) {
                return this.index[this.names[this.names_l.indexOf(pname)]]
            }
          }
    }
    
    res.get = function(k) {
        var name = this.names[k]
        return this.getValue(name)
    }

    res.getNum = function() {
        return this.names.length
    }
    return res
}
   
if (typeof window == 'undefined') {
    exports.createENUM = ENUM.createENUM
}

// $Id: gennc-common.js 101 2017-02-27 17:00:52Z jwillkomm $

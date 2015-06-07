// console.log('scanner script loading')

var P2X = P2X || {};

P2X.importObject = function(obj, target) {
    for (var k in obj) {
//        console.log('import: ' + k)
        target[k] = obj[k]
    }
}

P2X.getAttributeOrUndefined = function(elem, name) {
    var res
    if (elem.hasAttribute(name)) {
        res = elem.getAttribute(name)
    }
    return res
}

P2X.evalOrValue = function(val, def) {
    var res
    try {
        res = eval(val)
    } catch (err) {
        // res = val
    }
    // console.log('eval val: ' + typeof val + ' ' + val)
    // console.log('eval res: ' + typeof res + ' ' + res)
    if (typeof res == 'undefined') {
        res = def
    }
    return res
}

var escapeRegExp = function(str){
    return str
        .replace(/[.*+\?^${}()|[\]\\]/g, "\\$&")
        .replace(/\n/g, "\\n").replace(/\r/g, "\\r")
        .replace(/\t/g, "\\t").replace(/\v/g, "\\v")
}

var unescapeRegExp = function(str){
    return str
        .replace(/\\n/g, "\n").replace(/\\r/g, "\r")
        .replace(/\\t/g, "\t").replace(/\\v/g, "\v")
        .replace(/\\([.*+?^${}()|[\]\\])/g, "$1")
}

P2X.escapeBS = function(str, forQuote){
    var res = str
        .replace(/[\\]/g, "\\$&")
        .replace(/\n/g, "\\n").replace(/\r/g, "\\r")
        .replace(/\t/g, "\\t").replace(/\v/g, "\\v")
    if (forQuote != '\'') {
        res = res.replace(/\"/g, '\\"')
    }
    if (forQuote != "\"") {
        res = res.replace(/\'/g, '\\\'')
    }
    return res
}

P2X.escapeBSQLines = function(str){
    var escbs = P2X.escapeBS(str, '\'')
    return '\'' + escbs.replace(/\\n((\\n)+|.)/g, "\\n'\n+'$1") + '\''
}

var escapeXML = function(str){
    return String(str).replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
}

if (typeof window == 'undefined') {
    
    var fs = require('fs')
    var mod_assert = require('assert')
    var pXML = require('./parse-xml.js')

    var ENUM = {}
    ENUM.ParserMode = require('./modes.ncd.js')
    P2X.importObject(ENUM.ParserMode, ENUM)
    ENUM.ParserToken = require('./token.ncd.js')
    P2X.importObject(ENUM.ParserToken, ENUM)
    ENUM.ParserAssoc = require('./assoc.ncd.js')
    P2X.importObject(ENUM.ParserAssoc, ENUM)

    var assert = function(cond, msg) {
        mod_assert(cond, msg)
    }
} else {
    var assert = function(cond, msg) {
        console.assert(cond, msg)
    }
    global = window
}

var parseXml = pXML.parseXml
var not = function(x) { return !x; }
var exit = function(x) { assert(false, x); }

P2X.importObject(ENUM.ParserMode, ENUM)
P2X.importObject(ENUM.ParserToken, ENUM)
P2X.importObject(ENUM.ParserAssoc, ENUM)

P2X.importObject(ENUM.ParserMode, global)
P2X.importObject(ENUM.ParserToken, global)
P2X.importObject(ENUM.ParserAssoc, global)

// console.log('this: ' + this)
// console.dir(this)

// console.log('ENUM: ' + ENUM.ParserMode.MODE_IGNORE)
// console.log('ENUM: ' + ENUM.MODE_IGNORE)
// console.log('ENUM: ' + MODE_IGNORE)

// //console.dir(ENUM)
// console.log('MODE_IGNORE: ' + ENUM.MODE_IGNORE)
// console.log('MODE_IGNORE: ' + ENUM.MODE_IGNORE)
// console.log('ASSOC_LEFT: ' + ENUM.ASSOC_LEFT)
// console.log('TOKEN_FULL_STOP: ' + ENUM.TOKEN_FULL_STOP)

function arrayMin(arr) {
    var len = arr.length, min = Infinity, mink = -1;
  while (len--) {
    if (arr[len] < min) {
        min = arr[len];
        mink = len-1;
    }
  }
  return [min, mink];
};

function arrayMax(arr) {
  var max = -Infinity, maxk = -1;
  for (len = 0; len < arr.length; ++len) {
    if (arr[len] > max) {
      max = arr[len];
      maxk = len;
    }
  }
  return [max, maxk];
};

P2X.Token = function(tk, text, index, line, col, rule) {
    if (typeof tk != 'number') {
        console.log('warning: str conversion')
        tk = ENUM.getParserTokenValue(String(tk))
    }
    return { token: tk, tokenName: ENUM.ParserToken.names_index[tk], text: text, index: index, line: line, col: col }
}

P2X.TokenList = function(list, producer) {
    // console.log('TokenList created')
    this.name = 'test'
    this.list = list
    this.index = 0
    this.scanner = producer
}

P2X.TokenList.prototype.next = function() {
    return this.list[this.index++]
}
P2X.TokenList.prototype.mkeof = function() {
    var last = this.list[this.list.length-1], eof
    if (last) {
        eof = P2X.Token(TOKEN_EOF, '', last.index + last.text.length, last.line, last.col + last.text.length)
    } else {
        eof = P2X.Token(TOKEN_EOF, '', 0, 1, 0)
    }
    var res = new P2X.TokenList([].concat(this.list, [eof]))
    res.scanner = this.scanner
    return res
}
P2X.TokenList.prototype.mkroot = function() {
    // return new P2X.TokenList([].concat([P2X.Token(TOKEN_ROOT, '', 0, 0, 0)], this.list))
    return this
}
P2X.TokenList.prototype.charasxml = function(s) {
    if (s == "\n") {
        return "<ca:br/>"
    } else if (s == "\r") {
        return "<ca:cr/>"
    } else {
        return '<ca:text>' + escapeXML(s) + '</ca:text>'
    }
}
P2X.TokenList.prototype.asxml = function(indent) {
    var r = this.list, bindent
    if (typeof indent != 'string') {
        indent = ''
        bindent = ''
    } else {
        bindent = indent
        indent += ' '
    }
    var s = bindent + "<scan-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'>\n"
    if (this.scanner) {
        s += indent + '<input>' + escapeXML(this.scanner.input) + '</input>\n'
        s += this.scanner.get().asxml(indent)
    }
    for (var k in r) {
        var t = r[k]
        if (t.token) {
            s += indent + '<token'
                + ' line="' + t.line + '"'
                + ' col="' + t.col + '"'
                + ' index="' + t.index + '"'
                + ' type="' + ENUM.ParserToken.names_index[t.token] + '">' + this.charasxml(t.text) + '</token>\n';
        } else {
            if (false) {
                s += indent + '<ca:ignored'
                    + ' line="' + t.line + '"'
                    + ' col="' + t.col + '"'
                    + ' index="' + t.index + '"'
                    + ' >' + this.charasxml(t.text) + '</ca:ignored>\n';
            }
        }
    }
    s += bindent + '</scan-xml>\n'
    return s
}
P2X.TokenList.prototype.loadXML = function(scanListXML) {
    var scanDoc = parseXml(scanListXML)
    switch (scanDoc.documentElement.nodeName) {
    case 'scan-xml':
        return this.loadXMLNode(scanDoc.documentElement)
        break
    default:
        console.error('unexpected doc element name: ' + scanDoc.documentElement)
        console.error('unexpected doc element contains: ' + scanDoc.documentElement.firstChild.nodeValue)
        return null
        break
    }
}
P2X.TokenList.prototype.loadXMLNode = function(scanList) {
    var rlist = [], ctoken
    for (var k in scanList.childNodes) {
        ctoken = scanList.childNodes[k]
        if (ctoken.nodeType == 1 && ctoken.nodeName == "token") {
            catext = null
            for (j in ctoken.childNodes) {
                cctoken = ctoken.childNodes[j]
                text = ''
                if (cctoken.nodeType == 1 && cctoken.nodeName == "ca:text") {
                    catext = cctoken
                    if (catext.childNodes.length > 0
                        && catext.childNodes[0].nodeType == 3) {
                        text = catext.childNodes[0].nodeValue
                    }
                    break
                } else if (cctoken.nodeType == 1 && cctoken.nodeName == "ca:br") {
                    catext = cctoken
                    text = '\n'
                    break
                } else if (cctoken.nodeType == 1 && cctoken.nodeName == "ca:cr") {
                    catext = cctoken
                    text = '\r'
                    break
                }
            }
            rlist.push(P2X.Token(ctoken.getAttribute('type'),
                                 text,
                                 ctoken.getAttribute('index'),
                                 ctoken.getAttribute('line'),
                                 ctoken.getAttribute('col')))
        }
    }
    return new P2X.TokenList(rlist)
}

P2X.ScannerConfig = function(x) {
    var res = Array.apply({}, x);

    function getREString(re) {
        if (typeof re == 'object') {
            return re.source
        } else {
            return re
        }
    }
        
    function getREStringQ(re) {
        var s = getREString(re)
        if (s.indexOf('\'') > -1) {
            re = /'/
            s = s.replace(re, '\\\'')
        }
        return '\'' + s + '\''
    }
        
    res.asjson = function(indent) {

        if (typeof indent == 'undefined') indent = ''
        var res = indent + '['
        for (var k = 0; k < this.length; ++k) {
            var rule = this[k]
            var val = rule.action, actstr = ''
            if (val in ENUM.ParserToken.names_index) {
                actstr = ENUM.getParserTokenName(val)
            } else {
                actstr = val
            }
            // res += indent + '{re:\'' + P2X.escapeBS(getREString(rule.re)) + '\',action:'
            //     + actstr + '},\n'
            res += indent + JSON.stringify({re: getREString(rule.re), action: actstr}) + ',\n'
        }
        res += indent + ']\n'
        return res
    }

    res.asxml = function(indent) {

        function getREString(re) {
            if (typeof re == 'object') {
                return re.source
            } else {
                return re
            }
        }
        
        if (typeof indent == 'undefined') indent = ' '
        var res = indent + '<ca:scanner'
        if (this.type)
            res += ' type="' + this.type + this.length + '"'
        res += '>\n'
        for (var k = 0; k < this.length; ++k) {
            var rule = this[k]
            var val = rule.action, actstr = ''
            if (val in ENUM.ParserToken.names_index) {
                actstr = ENUM.ParserToken.prefix + ENUM.getParserTokenName(val)
            } else {
                actstr = escapeXML(val)
            }
            res += indent + indent + '<ca:lexem'
            res += '>'
            res += '<ca:re>' + escapeXML(getREString(rule.re)) + '</ca:re>'
            res += '<ca:action>' + actstr + '</ca:action>'
            res += '</ca:lexem>\n'
        }
        res += indent + '</ca:scanner>\n'
        return res
    }

    res.aslist = function() {
        var res = []
        for (var k = 0; k < this.length; ++k) {
            res[k] = this[k]
        }
        return res
    }

    res.loadXML = function(scConfXML) {
        var scanDoc = parseXml(scConfXML)
        switch (scanDoc.documentElement.nodeName) {
        case 'ca:scanner':
            return this.loadXMLDoc(scanDoc.documentElement)
            break
        default:
            console.error('p2x: unexpected doc element name: ' + scanDoc.documentElement.nodeName)
            console.error('p2x: unexpected doc element contains: ' + scanDoc.documentElement.firstChild.nodeValue)
            return null
            break
        }
    }
    res.loadXMLDoc = function(scConfTT) {
        // chead = scanList.getElementsByTagName('parser')
        // return this.loadXMLNode(chead[0])
        return this.loadXMLNode(scConfTT)
    }
    res.loadXMLNode = function(scanList) {
        var rlist = [], ctoken, eres_re, eres_action
        function getChildText(node, childName) {
            var res
            for (var k in node.childNodes) {
                var childN = node.childNodes[k]
                if (childN.nodeType == 1 && childN.nodeName == childName) {
                    for (var j in childN.childNodes) {
                        var childT = childN.childNodes[j]
                        if (childT.nodeType == 3) {
                            res = childT.nodeValue
                            break
                        }
                    }
                    break
                }
            }
            return res
        }
        for (var k in scanList.childNodes) {
            ctoken = scanList.childNodes[k], eres_re, eres_action
            if (ctoken.nodeType == 1 && ctoken.nodeName == "ca:lexem") {
                eres_re = ctoken.getAttribute('re') || getChildText(ctoken, 'ca:re')
                eres_action = ctoken.getAttribute('action') || getChildText(ctoken, 'ca:action')
                if (eres_re != 'undefined' && eres_re != '' && typeof eres_action != 'undefined') {
                    rlist.push({ re: eres_re, action: eres_action })
                }
            }
        }
        return new P2X.ScannerConfig(rlist)
    }

    for (var k in res) {
        var val = res[k]
        if (typeof k == 'function') {
            Object.defineProperty(res, k, { enumerable: false, value: val })
        }
    }
    
    return res
}

P2X.JScanner = function(name) {
    // console.log('JScanner created')
    return {
        name: name,
        actions: [],
        action_dir: {},
        token: [],
        input: '',
        yyindex: 0,
        yyline: 1,
        yycol: 0,
        yytext: '',
        yyignored: {},
        yyblocked: null,
        add: function(re, action) {
            var fst_val, snd_val, snd_source, entry
            if (re == '') {
                console.error('Error: Scanner: add: The RE must not be empty')
                return this
            }
            if (typeof re != 'object') {
                fst_val = RegExp(re)
            } else {
                fst_val = re
            }
            snd_val = action
            snd_source = snd_val
            if (typeof snd_val != 'number')
                snd_val = ENUM.getParserTokenValue(String(snd_val))
            entry = [fst_val, snd_val]
            entry.snd_source = snd_source
            entry.count = 0
            this.action_dir[re.source] = entry
            this.actions.push(entry)
            return this
        },
        set: function(scconf) {
            if (scconf.ignored) {
                this.include_ignored = true
            }
            if (scconf.rules) {
                return this.set(scconf.rules)
            }
            this.actions = []
            this.action_dir = {}
            for (var k = 0; k < scconf.length; ++k) {
                this.add(scconf[k].re, scconf[k].action)
            }
            return this
        },
        get: function() {
            scconf = Array(this.actions.length)
            for (var k in this.actions) {
                scconf[k] = {re: this.actions[k][0].source, action: this.actions[k].snd_source}
            }
            return P2X.ScannerConfig(scconf)
        },
        str: function(str) {
            this.input = str;
            this.token = []
            this.yyindex = 0
            this.yyline = 1
            this.yycol = 0
            this.yytext = ''
            this.actions.map(function (x) { x.blocked = false })
            return this
        },
        lex: function() {
            var re_nl = /\r/
            var matches = []
            var scanner = this
            var sinp = scanner.input.substr(this.yyindex)
            if (!sinp) return null
            var starts = []

            var results = this.actions.map(function (x) {
                if (x.blocked)
                    return null
                var sr = x[0].exec(sinp)
                if (sr) {
                    starts.push(sr.index)
                } else {
                    starts.push(undefined)
                }
                return sr
            })
            var max = -Infinity, maxk = -1;

            var min = arrayMin(starts);

            var self = this;

            if (min[0] == Infinity) {
                this.yyignored = P2X.Token(TOKEN_IGNORE, sinp, self.yyindex, self.yyline, self.yycol, first)
                return null
            }

            var lengths = []
            var atStart = results.map(function (x) {
                if (x && x.index == min[0]) {
                    lengths.push(x[0].length)
                    return x
                } else {
                    lengths.push(-1)
                    return null
                }
            })

            max = arrayMax(lengths);
            var first = max[1]

            hit = atStart[first];

            ++this.actions[first].count
            
            var tt = ENUM.ParserToken
            tt.act = this.actions[first][1];

            this.yyignored = {}
            
            if (min[0] > 0) {
                var skipped = sinp.substring(0, min[0])
                this.yyignored = P2X.Token(TOKEN_IGNORE, skipped, self.yyindex, self.yyline, self.yycol, first)
                var lnlind = skipped.lastIndexOf('\n')
                var lcrind = skipped.lastIndexOf('\r')
                if (lnlind > -1) {
                    ++this.yyline
                    this.yycol = min[0] - Math.max(lnlind, lcrind) - 1
                } else if (lcrind > -1) {
                    this.yycol = min[0] - lcrind - 1
                } else {
                    this.yycol = this.yycol + min[0]
                }
                self.yyindex += min[0]
            }

            res = P2X.Token(tt.act, atStart[first][0], self.yyindex, self.yyline, self.yycol, first)
            
            this.yytext = atStart[first][0]
            // this.yyindex = atStart[first].index + atStart[first][0].length
            this.yyindex += this.yytext.length

            if (this.yytext.length == 0) {
                this.actions[first].blocked = true
            } else {
                this.actions.map(function(x){x.blocked = false})
            }

            var lnlind = this.yytext.lastIndexOf('\n')
            var lcrind = this.yytext.lastIndexOf('\r')
            if (lnlind != -1) {
                ++this.yyline
                this.yycol = this.yytext.length - Math.max(lnlind, lcrind) - 1
            } else if (lcrind > -1) {
                this.yycol = this.yytext.length - lcrind - 1
            } else {
                this.yycol = this.yycol + this.yytext.length
            }

            return res
        },
        lexall: function() {
            var c
            this.alllist = []
            this.token = []
            this.ignored = []
            do {
                c = this.lex();
                if (this.yyignored.text) {
                    this.ignored.push(this.yyignored)
                    this.alllist.push(this.yyignored)
                }
                if (c) {
                    this.token.push(c)
                    this.alllist.push(c)
                }
            } while (c)
            if (this.include_ignored) {
                return new P2X.TokenList(this.alllist, this)
            } else {
                return new P2X.TokenList(this.token, this)
            }
        },
        asxml: function() {
            var r = this.lexall()
            return r.asxml();
        }
    }
}
  
var isOp = function(mode) { 
    return (mode == MODE_BINARY
            || mode == MODE_UNARY_BINARY
            || mode == MODE_UNARY
            || mode == MODE_POSTFIX)
}

P2X.TokenProto = function(tk, repr, mode, assoc, prec, precU, isParen, closingList) {
    var res
    if (typeof tk == 'object')
        res = P2X.TokenProto(tk.token, tk.repr, tk.mode, tk.assoc, tk.prec, tk.precU, tk.isParen, tk.closingList) 
    else {
        if (!tk in ENUM.ParserToken.names_index) {
            console.error('Value token ' + tk + ' must be in the set of allowed token: ')
            console.dir(ENUM.ParserToken.names_index)
            assert(false)
        }
        if (tk != TOKEN_IDENTIFIER)
            repr = ''
        if (closingList) {
            closingList = closingList.map(function(x) {
                return P2X.TokenProto(x)
            })
        }
        res = {
            token: tk, repr: repr, mode: mode || MODE_ITEM, assoc: assoc || ASSOC_NONE,
            prec: prec, precU: precU || 0,
            isParen: isParen || false,
            closingList: closingList || [],
        }
    }
    return res
}

P2X.TokenProtoRW = function() {
    var res = {}
    res.asxml = function(obj, indent) {
        if (!indent) indent = ' '
        var res = ''
        res += indent + '<ca:op'
        res += ' type="' + ENUM.getParserTokenName(obj.token) + '"'
        if (obj.token == TOKEN_IDENTIFIER && obj.repr)
            res += ' repr="' + obj.repr + '"'
        if (typeof obj.mode != 'undefined')
            res += ' mode="' + ENUM.getParserModeName(obj.mode) + '"'
        if (obj.mode == MODE_UNARY_BINARY || obj.mode == MODE_BINARY)
            res += ' associativity="' + ENUM.getParserAssocName(obj.assoc) + '"'
        if (typeof obj.prec != 'undefined')
            res += ' precedence="' + obj.prec + '"'
        if (obj.mode == MODE_UNARY_BINARY)
            res += ' unary-precedence="' + obj.precU + '"'
        if (obj.isParen)
            res += ' is-paren="1"'
        if (obj.closingList && obj.closingList.length > 0) {
            res += '>\n'
            res += indent + ' <ca:closing-list>\n'
            for (var k = 0; k < obj.closingList.length; ++k) {
                res += this.asxml(obj.closingList[k], indent + '  ')
            }
            res += indent + ' </ca:closing-list>\n'
            res += indent + '</ca:op>\n'
        } else {
            res += '/>\n'
        }
        return res
    }

    res.asJSON = function(obj, indent) {
        if (!indent) indent = ' '
        var res = ''
        res += '{'
        res += ' type: TOKEN_' + ENUM.getParserTokenName(obj.token) + ''
        if (obj.repr)
            res += ', repr: "' + obj.repr + '"'
        if (typeof obj.mode != 'undefined')
            res += ', mode: MODE_' + ENUM.getParserModeName(obj.mode) + ''
        if (obj.mode == MODE_UNARY_BINARY || obj.mode == MODE_BINARY)
            res += ', assoc: ASSOC_' + ENUM.getParserAssocName(obj.assoc) + ''
        if (typeof obj.prec != 'undefined')
            res += ', prec: ' + obj.prec + ''
        if (obj.mode == MODE_UNARY_BINARY)
            res += ', precU: ' + obj.precU + ''
        if (obj.isParen)
            res += ', isParen: 1'
        if (obj.closingList && obj.closingList.length > 0) {
            res += ',\n' + indent + '  closingList: [\n'
            for (var k = 0; k < obj.closingList.length; ++k) {
                if (k > 0)
                    res += ',\n'
                res += indent + '  ' + this.asJSON(obj.closingList[k], indent + '   ')
            }
            res += indent + '  ]\n'
            res += indent + '}'
        } else {
            res += ' }'
        }
        return res
    }

    res.loadXMLNode = function(tProtoNode) {
        var closingList
        var isParen = P2X.evalOrValue(P2X.getAttributeOrUndefined(tProtoNode, 'is-paren'), false)
        if (isParen) {
            closingList = []
            ch = tProtoNode.getElementsByTagName('ca:op')
            for (var k = 0; k < ch.length; ++k) {
                closingList[k] = this.loadXMLNode(ch[k])
                closingList[k] = P2X.TokenProto(closingList[k].token, closingList[k].repr)
            }
        }
        var res = {
            token: P2X.evalOrValue(ENUM.ParserToken.prefix + P2X.getAttributeOrUndefined(tProtoNode, 'type')),
            repr: P2X.getAttributeOrUndefined(tProtoNode, 'repr'),
            mode: P2X.evalOrValue(ENUM.ParserMode.prefix + P2X.getAttributeOrUndefined(tProtoNode, 'mode')),
            assoc: P2X.evalOrValue(ENUM.ParserAssoc.prefix + P2X.getAttributeOrUndefined(tProtoNode, 'associativity')),
            prec: P2X.evalOrValue(P2X.getAttributeOrUndefined(tProtoNode, 'precedence')),
            precU: P2X.evalOrValue(P2X.getAttributeOrUndefined(tProtoNode, 'unary-precedence')),
            isParen: isParen,
            closingList: closingList,
        }
        // console.log('load TP XML: ')
        // console.log(res)
        return P2X.TokenProto(res)
    }

    return res
}

P2X.ParserConfig = function(x) {
    res = Array.apply({}, x)
    return res
}

P2X.ParserConfigRW = function(x) {
    res = {}
    res.asxml = function(obj, indent) {
        if (!indent) indent = ' '
        var res = ''
        var tprw = P2X.TokenProtoRW()
        res += indent + '<ca:parser>\n'
        for (var k = 0; k < obj.length; ++k) {
            res += tprw.asxml(obj[k], indent + ' ')
        }
        res += indent + '</ca:parser>\n'
        return res
    }
    res.asJSON = function(obj, indent) {
        if (typeof indent == 'undefined') indent = ' '
        var res = ''
        var tprw = P2X.TokenProtoRW()
        res += indent + '[\n'
        for (var k = 0; k < obj.length; ++k) {
            if (k > 0)
                res += ',\n'
            res += indent + ' ' + tprw.asJSON(obj[k], indent + ' ')
        }
        res += indent + ']\n'
        return res
    }
    res.loadXML = function(scConfXML) {
        var scanDoc = parseXml(scConfXML)
        return this.loadXMLDoc(scanDoc)
    }
    res.loadXMLDoc = function(scanList) {
        chead = scanList.getElementsByTagName('ca:parser')
        if (chead.length) {
            return this.loadXMLNode(chead[0])
        } else {
            console.error('cannot find ca:parser in document, doc element name: ' + scanList.documentElement.nodeName)
            console.error('unexpected doc element contains: ' + scanList.documentElement.firstChild.nodeValue)
        }
    }
    res.loadXMLNode = function(scanList) {
        var rlist = [], ctoken, eres_re, eres_action
        var tprw = P2X.TokenProtoRW()
        for (var k in scanList.childNodes) {
            ctoken = scanList.childNodes[k], eres_re, eres_action
            if (ctoken.nodeType == 1 && ctoken.nodeName == "ca:op") {
                tProto = tprw.loadXMLNode(ctoken)
                rlist.push(tProto)
            }
        }
        return new P2X.ParserConfig(rlist)
    }
    return res
}

P2X.TokenInfo = function() {
    var tokenClasses = {}
    tokenClasses[TOKEN_JUXTA] = P2X.TokenProto({
        token: TOKEN_JUXTA, repr: '', mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 2, isParen: false
    })
    tokenClasses[TOKEN_ROOT] = P2X.TokenProto({
        token: TOKEN_ROOT, repr: '', mode: MODE_UNARY, prec: 1, isParen: false
    })
    tokenClasses[TOKEN_IGNORE] = P2X.TokenProto({
        token: TOKEN_IGNORE, mode: MODE_IGNORE
    })
    return {
        tokenClasses: tokenClasses,
        tokenClassesDefault: tokenClasses,
        seen: {},
        opCodes: 0,
        defaultInfo: P2X.TokenProto({
            token: TOKEN_EOF, repr: 'unknown', mode: MODE_ITEM, assoc: ASSOC_NONE, prec: 1e10, precU: 1e10, isParen: false
        }),
        tokenTypeEqual: function (s, t) {
            return s.token == t.token
                && (! (this.isNamedType(s) || this.isNamedType(t)) || s.text == t.text)
        },
        isNamedType: function (t) {
            if (typeof tl == 'object' && 'token' in tl) {
                res = this.isNamedType(tl.token)
            } else {
                res = !(t in ENUM.ParserToken.names_index)
            }
            return res
        },
        isOp: function(t) { 
            return isOp(this.mode(t));
        },
        mode: function (tl) {
            return this.get(tl).mode || MODE_ITEM
        },
        endList: function (tl) {
            return this.get(tl).closingList || [ this.getOpCode(TOKEN_EOF) ]
        },
        assoc: function (tl) {
            return this.get(tl).assoc || ASSOC_NONE
        },
        binary_prec: function (tl) {
            return this.get(tl).prec || 2
        },
        unary_prec: function (tl) {
            return this.get(tl).precU || 2
        },
        prec: function (tl) {
            return this.binary_prec(tl)
        },
        precUnary: function (tl) {
            return this.unary_prec(tl)
        },
        precedence: function (tl) {
            var tp = this.get(tl)
            if (tp.mode == MODE_UNARY_BINARY && !tl.left)
                return this.unary_prec(tl)
            else
                return this.binary_prec(tl)
        },
        isParen: function (tl) {
            return this.get(tl).isParen || false
        },
        getOpCode: function (tl, repr) {
            var res
            if (typeof tl == 'object' && 'token' in tl) {
                res = this.getOpCode(tl.token, tl.text)
            } else if (tl == TOKEN_IDENTIFIER && repr in this.seen) {
                res = this.seen[repr]
            } else if (tl in ENUM.ParserToken.names_index) { // case TOKEN_IDENTIFIER handled before
                res = tl
            } else if (typeof tl == 'string') {
                if (!(tl in this.seen)) {
                    this.seen[tl] = ++this.opCodes + 1000
                }
                res = this.seen[tl]
            } else {
            }
            return res
        },
        get: function (tl) {
            var opc = this.getOpCode(tl)
            if (opc in this.tokenClasses) {
                return this.tokenClasses[opc]
            } else {
                var res = this.defaultInfo
                return res
            }
        },
        getconfig: function () {
            res = Array(Object.keys(this.tokenClasses).length)
            n = 0
            for (var k in this.tokenClasses) {
                res[n++] = this.tokenClasses[k]
            }
            return P2X.ParserConfig(res)
        },
        setconfig: function (pc) {
            if (pc.rules) {
                return this.setconfig(pc.rules)
            }
            var patchTokenFunc = function(x){
                if (typeof x.token == "undefined" && typeof x.repr == "string" && x.length > 0) {
                    x.token == TOKEN_IDENTIFIER
                }
                if (typeof x.type != "undefined") {
                    x.token = x.type
                }
                if (typeof x.closingList != "undefined") {
                    x.closingList.map(patchTokenFunc)
                }
            }
            pc.map(patchTokenFunc)
            this.tokenClasses = this.tokenClassesDefault
            for (var k in pc) {
                this.insert(pc[k])
            }
            return this
        },
        normalize: function() {
            for (var k in this.tokenClasses) {
                this.tokenClasses[k] = P2X.TokenProto(this.tokenClasses[k])
            }
            return this
        },
        insert: function (tokenProto) {
            if (tokenProto.token == TOKEN_IDENTIFIER && tokenProto.repr) {
                // this creates the new op code
                // console.log('inserting token proto for named ID ' + tokenProto.repr + ': ' + this.getOpCode(tokenProto.repr))
                // console.log(' prec ' + tokenProto.prec)
                this.tokenClasses[this.getOpCode(tokenProto.repr)] = tokenProto
            } else if (typeof tokenProto.token == "undefined") {
            } else {
                if (tokenProto.token == TOKEN_JUXTA) {
                    // it's allowed to set a new rule for TOKEN_JUXTA,
                    // but it must be MODE_BINARY
                    assert(tokenProto.mode == MODE_BINARY)
                } else if (tokenProto.token == TOKEN_ROOT) {
                    // it must be MODE_UNARY, with prec 1
                    assert(tokenProto.mode == MODE_UNARY)
                    assert(tokenProto.prec == 1)
                }
                this.tokenClasses[this.getOpCode(tokenProto.token)] = tokenProto
            }
            return this
        },
    }
}

P2X.Parser = function(tokenInfo) {
    if (typeof tokenInfo == "undefined")
        tokenInfo = P2X.TokenInfo()
    return {
        root: undefined,
        input: undefined,
        tokenInfo: tokenInfo,
        endList: undefined,
        options: {
            ignoreIgnore: false
        },
        getconfig: function () {
            return this.tokenInfo.getconfig()
        },
        setconfig: function (pc) {
            this.tokenInfo.setconfig(pc)
            if ('ignoreIgnore' in pc) {
                this.options.ignoreIgnore = pc.ignoreIgnore
            }
            return this
        },
        mkroot: function() {
            return P2X.Token(TOKEN_ROOT, '', 0, 0, 0);
        },
        mkJuxta: function(t) {
            var res = P2X.Token(TOKEN_JUXTA, '', t.index, t.line, t.col, t.index);
            return res
        },
        getRM: function(t) {
            while (t.right) {
                t = t.right;
            }
            return t;
        },
        getRMOp: function() {
            t = this.root
            while (t.right && this.tokenInfo.isOp(t.right)) {
                t = t.right
            }
            return t
        },
        pushBinary: function(t) {
            var tmp = this.root
            var parent = null
            var assoc = this.tokenInfo.assoc(t)
            var prec = this.tokenInfo.binary_prec(t)
            // console.log('pushBinary: enter')
            // console.dir(tmp)
            // console.log('pushBinary: mode tmp: ' + ENUM.getParserModeName(this.tokenInfo.mode(tmp)) + ' prec ' + this.tokenInfo.prec(tmp))
            while (tmp.right
                   && ((this.tokenInfo.precedence(tmp) < prec && this.tokenInfo.mode(tmp) != MODE_POSTFIX) || 
                       (this.tokenInfo.tokenTypeEqual(tmp, t) && assoc == ASSOC_RIGHT))) {
                // console.log('pushBinary: in while: ')
                // console.dir(tmp)
                parent = tmp;
                tmp = tmp.right;
            }
            if ((this.tokenInfo.precedence(tmp) < prec && this.tokenInfo.mode(tmp) != MODE_POSTFIX) ||
                (this.tokenInfo.tokenTypeEqual(tmp, t) && assoc == ASSOC_RIGHT)) {
                assert(tmp.right == undefined);
                tmp.right = t;
            } else {
                // console.log('pushBinary: ins+rotate ')
                // console.dir(parent)
                // console.dir(tmp)
                t.left = tmp;
                if (tmp === this.root) {
                    assert(parent == undefined);
                    this.root = t;
                } else {
                    assert(parent != undefined);
                    parent.right = t;
                }
            }
        },
        pushPostfix: function(t) {
            return this.pushBinary(t)
        },
        pushItem: function(t) {
            var rmop = this.getRMOp();
            // console.log('pushItem: RM op: ' + rmop)
            // console.dir(rmop)
            if (this.tokenInfo.mode(rmop) == MODE_POSTFIX || rmop.right) {
                op = this.mkJuxta(t);
                this.pushBinary(op);
                rmop2 = this.getRMOp();
                // console.log('pushItem: RM op II: ' + rmop)
                // console.dir(rmop)
                // console.dir(rmop2)
                assert(rmop2.right == undefined);
                assert(rmop2 === op);
                op.right = t;
            } else {
                rmop.right = t;
            }
            return this
        },
        pushUnary: function(t) {
            return this.pushItem(t)
        },
        pushUnaryBinary: function(t) {
            if (this.rightEdgeOpen()) {
                this.pushUnary(t);
            } else {
                this.pushBinary(t);
            }
        },
        rightEdgeOpen: function() {
            var rm = this.getRMOp();
            var res = (!rm.right) && (this.tokenInfo.mode(rm) != MODE_POSTFIX)
            return res
        },
        pushIgnore: function(t) {
            if (!this.options.ignoreIgnore) {
                // console.log('pushIgnore: ')
                // console.dir(t)
                var rm = this.getRM(this.root);
                while (rm.content) {
                    rm = this.getRM(rm.content);
                }
                // console.log('rm: ')
                t.ignore = rm.ignore;
                rm.ignore = t;
                // console.dir(rm)
            }
        },
        insertToken: function(first) {
            // console.log('insertToken: t: ')
            // console.dir(first)
            var firstMode = this.tokenInfo.mode(first)
            assert(not(firstMode & MODE_PAREN)); // MODE_PAREN bit is cleared
            assert(firstMode != 0); // mode is not 0
            assert(firstMode != MODE_PAREN); // mode is not MODE_PAREN
            switch(firstMode) {
            case MODE_ITEM:
                this.pushItem(first);
                break;
            case MODE_IGNORE:
                this.pushIgnore(first);
                break;
            case MODE_UNARY:
                this.pushUnary(first);
                break;
            case MODE_BINARY:
                this.pushBinary(first);
                break;
            case MODE_POSTFIX:
                this.pushPostfix(first);
                break;
            case MODE_UNARY_BINARY:
                this.pushUnaryBinary(first);
                break;
            default:
                console.error("error: parser: invalid mode " + firstMode + "\n");
                exit(1);
                break;
            }
            return this
        },
        parse: function(tlist) {
            if (typeof (this.endList) == "undefined") 
                this.endList = [this.tokenInfo.getOpCode(TOKEN_EOF)]
            this.root = this.mkroot()
            this.root.parser = this
            this.root.scanner = tlist.scanner
            this.input = tlist;

            var first

            var endFound = false;
            do {
                first = this.input.next()
                // console.log("Parser: next, text='" + first.text
                //             + "' code: " + this.tokenInfo.getOpCode(first)
                //             + ', mode: ' + ENUM.getParserModeName(this.tokenInfo.mode(first)) + ', prec: ' + this.tokenInfo.prec(first))
                // console.dir(first)
                if (typeof first == "undefined") {
                    console.error("Parser: unexpected end found, exiting")
                    console.dir(first)
                    endFound = true
                } else if (this.endList.indexOf(this.tokenInfo.getOpCode(first)) > -1) {
                    // console.log("Parser: end found: "+ this.endList + ' ' + this.tokenInfo.getOpCode(first.token))
                    endFound = true
                } else if (this.tokenInfo.isParen(first)) {
                    var parser = P2X.Parser(this.tokenInfo)

                    // console.log('sub parser endlist:')
                    // console.dir(this.tokenInfo.endList(first))
                    
                    parser.options = this.options
                    parser.endList = this.tokenInfo.endList(first).map(function(x){return x.token})

                    // console.dir(parser.endList)
                    
                    var last = parser.parse(this.input)
                    parser.pushIgnore(last);

                    this.insertToken(first)

                    if (parser.root.ignore) {
                        this.pushIgnore(parser.root.ignore);
                    }

                    first.content = parser.root.right
                    
                } else {
                    this.insertToken(first)
                }
            } while (! endFound && first)
            return first
        }
    }        
}

P2X.TreePrinterOptions = function() {
    return {
        parseConf: false,
        scanConf: false,
        treewriterConf: false,
        caSteps: true,
        id: false,
        line: true,
        col: true, 
        code: false,
        prec: true,
        mode: true,
        type: true,
        indent: true,
        newlineAsBr: true,
        merged: false,
        strict: false,
        encoding: 'utf-8'
    }
 }

P2X.TreePrinter = function(tokenInfo, tpOptions) {
    if (!tokenInfo) {
        tokenInfo = P2X.TokenInfo()
    }
    if (!tpOptions) {
        tpOptions = P2X.TreePrinterOptions()
    }
    return {
        name: 'testTreePrinter',
        tokenInfo: tokenInfo,
        options: tpOptions,
        asxml: function(t, indent, called) {
            var res = '';
            if (!indent) indent = ' '
            if (!called) {
                res += "<code-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/' ca:version='1.0'>\n"
                if (this.options.caSteps) {
                    res += indent + "<ca:steps/>\n"
                }
                if (this.options.parseConf) {
                    console.log('Scanner conf')
                    console.dir(t.scanner.get())
                    res += t.scanner.get().asxml()
                }
                if (this.options.parseConf) {
                    var pcwr = P2X.ParserConfigRW();
                    res += pcwr.asxml(t.parser.getconfig(), indent)
                }
                if (this.options.treewriterConf) {
                    res += indent + '<ca:tree-writer'
                    for (key in this.options) {
                        res += ' ' + key + '="' + this.options[key] + '"'
                    }
                    res += '/>\n'
                }
            }
            if (t) {
                var tagname = 'op'
                if (this.tokenInfo.isParen(t))
                    tagname = 'paren'
                else if (t.token == TOKEN_FLOAT)
                    tagname = 'float'
                else if (t.token == TOKEN_INTEGER)
                    tagname = 'int'
                else if (t.token == TOKEN_IDENTIFIER && !this.tokenInfo.isOp(t))
                    tagname = 'id'
                else if (t.token == TOKEN_ROOT)
                    tagname = 'root'
                res += indent + '<' + tagname
                res += this.writeXMLLocAttrs(t)
                res += this.writeXMLTypeAttrs(t)
                res += '>\n';
                if (t.left) {
                    res += this.asxml(t.left, indent + ' ', true);
                } else if (t.right || t.content) {
                    res += indent + ' <null/>\n';
                }
                res += this.writeXMLTextElem(t, indent + ' ')
                if (t.ignore) {
                    res += this.writeIgnoreXML(t.ignore, indent + ' ');
                }
                if (t.content) {
                    res += this.asxml(t.content, indent + ' ', true);
                } else if (t.right) {
                    if (this.options.strict)
                        res += indent + ' <null/>\n';
                }
                res += this.asxml(t.right, indent + ' ', true);
                res += indent + '</' + tagname + '>\n';
            }
            if (!called) {
                res += '</' + 'code-xml' + '>\n';
            }
            return res
        },
        writeXMLLocAttrs: function(t) {
            var res = ''
            if (t.line) {
                if (this.options.line) {
                    res += ' line="' + t.line + '"'
                }
                if (this.options.col) {
                    res += ' col="' + t.col + '"'
                }
            }
            return res
        },
        writeXMLTypeAttrs: function(t) {
            var res = ''
            if (t.text && t.token == TOKEN_IDENTIFIER)
                res += ' repr="' + t.text + '"'
            var ttext
            if (this.options.type) {
                if (t.token)
                    ttext = ENUM.ParserToken.names_index[t.token]
                else 
                    ttext = typeof t
                res += ' type="' + ttext + '"'
            }
            if (this.options.code) {
                res += ' code="' + this.tokenInfo.getOpCode(t) + '"'
            }
            return res
        },
        writeXMLTextElem: function(t, indent) {
            var res = ''
            var ttext = t.text || (t.token ? {text:''} : undefined) || t.toString() || string(t)
            if (typeof ttext == "object" && Object.keys(ttext).indexOf('text') > -1)
                ttext = ttext.text
            assert(typeof ttext == "string")
            if (ttext) {
                res = indent + P2X.TokenList.prototype.charasxml.apply({}, [ttext])
                if (indent)
                    res += '\n'
            }
            return res
        },
        writeIgnoreXML: function(t, indent) {
            var res = ''
            if (t.ignore) {
                res += this.writeIgnoreXML(t.ignore, indent)
            }
            res += indent + "<ca:ignore";
            if (this.options.id)
                res += " id='" << t.id << "'";
            res += this.writeXMLLocAttrs(t);
            res += this.writeXMLTypeAttrs(t) + '>';
            res += this.writeXMLTextElem(t, '')
            res += "</ca:ignore>\n";
            return res
        }
    }
}

P2X.parseJSONEval = function(text) {
    var result, code, XXX
    code = 'var XXX = ' + text
    try {
        eval(code)
    } catch (me) {
        console.error('Failed to parse config struct: ' + me)
        console.error('Bad code: ' + code)
        XXX = undefined
    }
    return XXX;
}

P2X.parseJSON = function(text) {
    var result, code, XXX
    code = 'var XXX = ' + text
    try {
        eval(code)
    } catch (me) {
        console.error('Failed to parse config struct: ' + me)
        console.error('Bad code: ' + code)
        XXX = undefined
    }
    return XXX;
}

P2X.p2xj = function(input, p2xConf, result) {
    var scanConf = p2xConf.scanner
    if (typeof result == "undefined") {
        result = {}
    }
    if (typeof scanConf != "object") {
        scanConf = P2X.parseJSON(scanConf)
        if (typeof scanConf == "undefined") {
            console.error('Failed to parse scanner config')
            scanConf = []
            result.error = 'Failed to parse scanner config'
        }
    }
    var scanner = P2X.JScanner()
    scanner.set(scanConf)

    if (p2xConf.debug) {
        result.scanconf = scanner.get()
    }
    
    var parseConf = p2xConf.parser
    if (typeof parseConf != "object") {
        parseConf = P2X.parseJSON(parseConf)
        if (typeof parseConf == "undefined") {
            console.error('Failed to parse parser config')
            parseConf = []
            result.error = 'Failed to parse parser config'
        }
    }

    var parser = P2X.Parser()
    parser.setconfig(parseConf)

    if (p2xConf.debug) {
        var pcrw = P2X.ParserConfigRW()
        result.parseconf = parser.getconfig()
    }
    
    scanner.str(input)
    var tl = scanner.lexall().mkeof()

    if (p2xConf.debug) {
        result.tokenlist = tl
    }

    var res = parser.parse(tl)

    if (p2xConf.debug) {
        result.parseres = parser.root
    }

    var tpOptions = p2xConf.treeprinter;
    var tp = P2X.TreePrinter(parser.tokenInfo, tpOptions)

    result.xmlres = tp.asxml(parser.root)

    if (p2xConf.debug) {
        result.scanner = scanner
        result.parser = parser
    }
    
    return result.xmlres
}

P2X.scanner = P2X.JScanner('p2x1main')

if (typeof window == 'undefined') {
    exports.JScanner = P2X.JScanner
    exports.ScannerConfig = P2X.ScannerConfig
    exports.scanner = P2X.scanner
    exports.Token = P2X.Token
    exports.TokenList = P2X.TokenList
    exports.TokenInfo = P2X.TokenInfo
    exports.TokenProto = P2X.TokenProto
    exports.TokenProtoRW = P2X.TokenProtoRW
    exports.ParserConfig = P2X.ParserConfig
    exports.ParserConfigRW = P2X.ParserConfigRW
    exports.Parser = P2X.Parser
    exports.TreePrinterOptions = P2X.TreePrinterOptions
    exports.TreePrinter = P2X.TreePrinter
    exports.importObject = P2X.importObject
    exports.p2xj = P2X.p2xj;
    exports.parseJSON = P2X.parseJSON;
    exports.escapeBS = P2X.escapeBS;
    exports.escapeBSQLines = P2X.escapeBSQLines;
    //exports.P2X = P2X
}

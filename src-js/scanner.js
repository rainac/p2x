var P2X = P2X || {};

if (typeof window == 'undefined') {
    P2X._HashMap = require('./hashmap.js')
    P2X.HashMap = P2X._HashMap.HashMap
    var pXML = require('./parse-xml.js')
    var parseXml = pXML.parseXml
}

P2X.maxPrec = 1e5

P2X.debug = P2X.debug || 0

P2X.importObject = function(obj, target) {
    for (var k in obj) {
        target[k] = obj[k]
    }
}

var escapeXML = function(str){
    return String(str).replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
}

if (typeof window == 'undefined') {
    var fs = require('fs')
    var mod_assert = require('assert')

    P2X.ENUM = {}
    P2X.ENUM.ParserMode = require('./modes.ncd.js')
    P2X.ENUM.ParserToken = require('./token.ncd.js')
    P2X.ENUM.ParserAssoc = require('./assoc.ncd.js')
    var assert = function(cond, msg) {
        mod_assert(cond, msg)
    }
} else {
    var assert = function(cond, msg) {
        console.assert(cond, msg)
    }
}

var not = function(x) { return !x; }
var exit = function(x) { assert(false, x); }

P2X.importObject(P2X.ENUM.ParserMode, P2X)
P2X.importObject(P2X.ENUM.ParserToken, P2X)
P2X.importObject(P2X.ENUM.ParserAssoc, P2X)
P2X.importObject(P2X.ENUM.ParserMode.ParserMode.symbols, P2X)
P2X.importObject(P2X.ENUM.ParserToken.ParserToken.symbols, P2X)
P2X.importObject(P2X.ENUM.ParserAssoc.ParserAssoc.symbols, P2X)

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
    if (typeof tk == 'object') {
        return P2X.Token(tk.token, tk.text, tk.index, tk.line, tk.col)
    } else if (typeof tk != 'number') {
        console.log('warning: str conversion')
        tk = P2X.ParserToken.getValue(String(tk))
    }
    return { token: tk, tokenName: P2X.ParserToken.names_index[tk], text: text, index: index, line: line, col: col }
}

P2X.TokenList = function(list, producer) {
    var obj = this
    obj.name = 'test'
    obj.list = list
    obj.index = 0
    obj.scanner = producer
    obj.caNSPref = 'ca:'
    obj.caTextName = obj.caNSPref + 'text'
    return obj
}

P2X.TokenList.prototype.next = function() {
    return this.list[this.index++]
}
P2X.TokenList.prototype.mkeof = function() {
    var last = this.list[this.list.length-1], eof
    if (last) {
        eof = P2X.Token(P2X.TOKEN_EOF, '', last.index + last.text.length, last.line, last.col + last.text.length)
    } else {
        eof = P2X.Token(P2X.TOKEN_EOF, '', 0, 1, 0)
    }
    var res = new P2X.TokenList([].concat(this.list, [eof]))
    res.scanner = this.scanner
    return res
}
P2X.TokenList.prototype.mkroot = function() {
    // return new P2X.TokenList([].concat([P2X.Token(P2X.TOKEN_ROOT, '', 0, 0, 0)], this.list))
    return this
}
P2X.TokenList.prototype.charasxml = function(s) {
    if (s == "\n") {
        return "<" + this.caNSPref + "br/>"
    } else if (s == "\r") {
        return "<" + this.caNSPref + "cr/>"
    } else {
        return '<' + this.caTextName + '>' + escapeXML(s) + '</' + this.caTextName + '>'
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
                + ' type="' + P2X.ParserToken.names_index[t.token] + '">' + this.charasxml(t.text) + '</token>\n';
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
    if (typeof x == 'object' && 'rules' in x) {
        var res = P2X.ScannerConfig(x.rules)
        Object.keys(x).map(function(k){
            res[k] = x[k]
        })
        return res
    }
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
            if (val in P2X.ParserToken.names_index) {
                actstr = P2X.ParserToken.getName(val)
            } else {
                actstr = val
            }
            // res += indent + '{re:\'' + P2X.escapeBS(getREString(rule.re)) + '\',action:'
            //     + actstr + '},\n'
            res += indent + JSON.stringify({re: getREString(rule.re), action: actstr}) + ',\n'
        }
        res += indent + ']\n'
        if (this.ignored) {
            res = '{rules:' + res + ',ignored:true}'
        }
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
            if (val in P2X.ParserToken.names_index) {
                actstr = P2X.ParserToken.prefix + P2X.ParserToken.getName(val)
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
                snd_val = P2X.ParserToken.getValue(String(snd_val))
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
            var scconf = Array(this.actions.length)
            for (var k in this.actions) {
                scconf[k] = {re: this.actions[k][0].source, action: this.actions[k].snd_source}
            }
            var res = P2X.ScannerConfig(scconf)
            if (this.include_ignored)
                res.ignored = true
            return res
        },
        asjson: function() {
            return this.get().asjson()
        },
        str: function(str, filename) {
            this.filename = filename;
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
            this.yyignored = {}
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
                this.yyignored = P2X.Token(P2X.TOKEN_IGNORE, sinp, self.yyindex, self.yyline, self.yycol, first)
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

            var tt = P2X.ParserToken
            tt.act = this.actions[first][1];

            if (min[0] > 0) {
                var skipped = sinp.substring(0, min[0])
                this.yyignored = P2X.Token(P2X.TOKEN_IGNORE, skipped, self.yyindex, self.yyline, self.yycol, first)
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
    return (mode == P2X.MODE_BINARY
            || mode == P2X.MODE_UNARY_BINARY
            || mode == P2X.MODE_UNARY
            || mode == P2X.MODE_POSTFIX)
}

P2X.TokenProto = function(tk, repr, mode, assoc, prec, precU, isParen, isRParen, closingList, name, ignoreIfStray) {
    var res
    if (typeof tk == 'object')
        res = P2X.TokenProto(tk.token, tk.repr, tk.mode, tk.assoc, tk.prec, tk.precU, tk.isParen, tk.isRParen, tk.closingList, tk.name, tk.ignoreIfStray)
    else {
        if (!tk in P2X.ParserToken.names_index) {
            console.error('Value token ' + tk + ' must be in the set of allowed token: ')
            console.dir(P2X.ParserToken.names_index)
            assert(false)
        }
        isParen = isParen ? true : false
        isRParen = isRParen ? true : false
        if (typeof token == 'string')
            token = P2X.ParserMode.getValue(token)
        if (typeof mode == 'string')
            mode = P2X.ParserMode.getValue(mode)
        if (typeof assoc == 'string')
            assoc = P2X.ParserAssoc.getValue(assoc)
        if (tk != P2X.TOKEN_IDENTIFIER)
            repr = ''
        if (closingList) {
            closingList = closingList.map(function(x) {
                var cli = x
                cli.mode = mode
                if (mode != P2X.MODE_ITEM) {
                    cli.prec = prec
                    cli.precU = precU
                    cli.assoc = assoc
                }
                cli.isParen = false
                cli.isRParen = true
                cli = P2X.TokenProto(cli)
                return cli
            })
        }
        if (!ignoreIfStray) {
            ignoreIfStray = false
        }
        var defAssoc = (mode == P2X.MODE_BINARY || mode == P2X.MODE_UNARY_BINARY) ? P2X.ASSOC_LEFT : P2X.ASSOC_NONE
        res = {
            token: tk, repr: repr,
            mode: mode || P2X.MODE_ITEM,
            assoc: assoc || defAssoc,
            prec: prec || 2,
            precU: precU || 2,
            isParen: isParen || false,
            isRParen: isRParen || false,
            closingList: closingList,
            name: name,
            ignoreIfStray: ignoreIfStray
        }
        if (res.mode == P2X.MODE_ITEM) {
            res.prec = res.precU = P2X.maxPrec
        }
        if (res.isRParen) {
            res.closingList = []
        }
        if (res.isParen) {
            res.closingList = res.closingList || [ P2X.TOKEN_EOF ]
        }
    }
    return res
}

P2X.ParserConfig = function(x) {
    res = Array.apply({}, x)
    return res
}

P2X.TokenInfo = function() {
    var tokenClasses = {}
    tokenClasses[P2X.TOKEN_JUXTA] = P2X.TokenProto({
        token: P2X.TOKEN_JUXTA, repr: '', mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 2, isParen: false
    })
    tokenClasses[P2X.TOKEN_ROOT] = P2X.TokenProto({
        token: P2X.TOKEN_ROOT, repr: '', mode: P2X.MODE_UNARY, prec: 1, isParen: false
    })
    tokenClasses[P2X.TOKEN_IGNORE] = P2X.TokenProto({
        token: P2X.TOKEN_IGNORE, mode: P2X.MODE_IGNORE
    })
    return {
        tokenClasses: tokenClasses,
        tokenClassesDefault: tokenClasses,
        seen: {},
        opCodes: 0,
        defaultInfo: P2X.TokenProto({
            token: P2X.TOKEN_EOF, repr: 'unknown', mode: P2X.MODE_ITEM, assoc: P2X.ASSOC_NONE, prec: P2X.maxPrec, precU: P2X.maxPrec, isParen: false
        }),
        tokenTypeEqual: function (s, t) {
            return s.token == t.token
                && (! (this.isNamedType(s) || this.isNamedType(t)) || s.text == t.text)
        },
        isNamedType: function (t) {
            if (typeof tl == 'object' && 'token' in tl) {
                res = this.isNamedType(tl.token)
            } else {
                res = !(t in P2X.ParserToken.names_index)
            }
            return res
        },
        isOp: function(t) {
            return isOp(this.mode(t));
        },
        tagName: function (tl) {
            return this.get(tl).name
        },
        mode: function (tl) {
            return this.get(tl).mode
        },
        ignoreIfStray: function (tl) {
            return this.get(tl).ignoreIfStray
        },
        endList: function (tl) {
            return this.get(tl).closingList
        },
        assoc: function (tl) {
            return this.get(tl).assoc
        },
        binary_prec: function (tl) {
            return this.get(tl).prec
        },
        unary_prec: function (tl) {
            var tp = this.get(tl), res
            if (tp.mode == P2X.MODE_UNARY) {
                res = tp.prec;
            } else if (tp.mode == P2X.MODE_UNARY_BINARY) {
                res = tp.precU;
            }
            return res
        },
        prec: function (tl) {
            return this.binary_prec(tl)
        },
        precUnary: function (tl) {
            return this.unary_prec(tl)
        },
        precedence: function (tl) {
            var tp = this.get(tl)
            if (tp.mode == P2X.MODE_UNARY_BINARY && !tl.left)
                return this.unary_prec(tl)
            else
                return this.binary_prec(tl)
        },
        isParen: function (tl) {
            return this.get(tl).isParen || this.get(tl).isRParen || false
        },
        isLParen: function (tl) {
            return this.get(tl).isParen || false
        },
        isRParen: function (tl) {
            return this.get(tl).isRParen || false
        },
        getOpCode: function (tl, repr) {
            var res
            if (typeof tl == 'object' && 'token' in tl) {
                res = this.getOpCode(tl.token, tl.text)
            } else if (tl == P2X.TOKEN_IDENTIFIER && repr in this.seen) {
                res = this.seen[repr]
            } else if (tl in P2X.ParserToken.names_index) { // case P2X.TOKEN_IDENTIFIER handled before
                res = tl
            } else if (typeof tl == 'string') {
                if (!(tl in this.seen)) {
                    this.seen[tl] = ++this.opCodes + 1000
                }
                res = this.seen[tl]
            } else {
                res = tl
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
                    x.token == P2X.TOKEN_IDENTIFIER
                }
                if (typeof x.type == "string") {
                    if (x.type.startsWith('TOKEN_')) {
                        x.type == x.type.substring(6)
                    }
                    x.type = P2X.ParserToken.index[x.type]
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
            var tpNormalized = P2X.TokenProto(tokenProto) // normalizes
            if (tpNormalized.closingList) {
                var this_ = this
                tpNormalized.closingList.map(function(k) {
                    if (k.token == P2X.TOKEN_IDENTIFIER && k.repr) {
                        this_.getOpCode(k.repr)
                    }
                    this_.tokenClasses[this_.getOpCode(k.token, k.repr)] = k
                })
            }
            if (tokenProto.token == P2X.TOKEN_IDENTIFIER && tokenProto.repr) {
                // this creates the new op code
                // console.log('inserting token proto for named ID ' + tokenProto.repr + ': ' + this.getOpCode(tokenProto.repr))
                // console.log(' prec ' + tokenProto.prec)
                this.tokenClasses[this.getOpCode(tokenProto.repr)] = tpNormalized
            } else if (typeof tokenProto.token == "undefined") {
            } else {
                if (tokenProto.token == P2X.TOKEN_JUXTA) {
                    // it's allowed to set a new rule for P2X.TOKEN_JUXTA,
                    // but it must be P2X.MODE_BINARY
                    assert(tokenProto.mode == P2X.MODE_BINARY)
                } else if (tokenProto.token == P2X.TOKEN_ROOT) {
                    // it must be P2X.MODE_UNARY, with prec 1
                    assert(tokenProto.mode == P2X.MODE_UNARY)
                    assert(tokenProto.prec == 1)
                }
                this.tokenClasses[this.getOpCode(tokenProto.token)] = tpNormalized
            }
            return this
        },
    }
}

P2X.Parser = function(tokenInfo) {
    if (typeof tokenInfo == "undefined")
        tokenInfo = P2X.TokenInfo()
    var res = {
        root: undefined,
        input: undefined,
        tokenInfo: tokenInfo,
        endList: undefined,
        options: {
            ignoreIgnore: false
        },
        leastMap: P2X.HashMap(P2X.maxPrec+1),
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
            return P2X.Token(P2X.TOKEN_ROOT, '', 0, 0, 0);
        },
        mkJuxta: function(t) {
            var res = P2X.Token(P2X.TOKEN_JUXTA, '', t.index, t.line, t.col, t.index);
            return res
        },
        getRM: function(t) {
            while (t.right) {
                t = t.right;
            }
            return t;
        },
        getRMOp: function() {
            var it = this.leastMap.end()
            --it
            if (this.tokenInfo.mode(this.leastMap.second(it)) == P2X.MODE_ITEM)
                --it
            return this.leastMap.second(it)
        },
        updateLeastMap: function(t, prec) {
            this.leastMap.insert(prec, t)
            this.leastMap.erase(1+this.leastMap.lower_bound(prec), this.leastMap.end())
        },
        pushBinary: function(t) {
            var tmp, parent
            var assoc = this.tokenInfo.assoc(t)
            var prec = this.tokenInfo.binary_prec(t)

            var it = this.leastMap.lower_bound(prec + (assoc == P2X.ASSOC_RIGHT ? 1 : 0));

            --it;
            while(this.tokenInfo.mode(this.leastMap.second(it)) == P2X.MODE_POSTFIX) --it;

            parent = this.leastMap.second(it);
            tmp = parent.right; // !!!

            parent.right = t;
            if (tmp)
                t.left = tmp;

            this.updateLeastMap(t, prec)
        },
        pushPostfix: function(t) {
            return this.pushBinary(t)
        },
        pushItem_: function(t) {
            var rmop = this.getRMOp();
            if (this.tokenInfo.mode(rmop) == P2X.MODE_POSTFIX || rmop.right) {
                var op = this.mkJuxta(t);
                this.pushBinary(op);
                var rmop2 = this.getRMOp();
                assert(rmop2.right == undefined);
                assert(rmop2 === op);
                op.right = t;
            } else {
                rmop.right = t;
            }
            return this
        },
        pushItem: function(t) {
            this.pushItem_(t)
            this.leastMap.insert(P2X.maxPrec, t)
        },
        pushUnary: function(t) {
            this.pushItem_(t)
            var prec = this.tokenInfo.unary_prec(t);
            this.updateLeastMap(t, prec)
            return this
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
            var res = (!rm.right) && (this.tokenInfo.mode(rm) != P2X.MODE_POSTFIX)
            return res
        },
        pushIgnoreNew: function(t) {
            if (!this.options.ignoreIgnore) {
                var rm = this.getRM(this.root)
                var tend = t
                while (tend.ignore)
                    tend = tend.ignore
                tend.ignore = rm.ignore
                rm.ignore = t
            }
        },
        pushIgnore: function(t) {
            if (!this.options.ignoreIgnore) {
                // console.log('pushIgnore: ')
                // console.dir(t)
                var rm = this.getRM(this.root);
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
            assert(not(firstMode & P2X.MODE_PAREN)); // P2X.MODE_PAREN bit is cleared
            assert(firstMode != 0); // mode is not 0
            assert(firstMode != P2X.MODE_PAREN); // mode is not P2X.MODE_PAREN
            if (firstMode == P2X.MODE_BINARY
                && this.tokenInfo.ignoreIfStray(first)
                && this.rightEdgeOpen()) {
                firstMode = P2X.MODE_IGNORE
            }
            switch(firstMode) {
            case P2X.MODE_ITEM:
                this.pushItem(first);
                break;
            case P2X.MODE_IGNORE:
                this.pushIgnore(first);
                break;
            case P2X.MODE_UNARY:
                this.pushUnary(first);
                break;
            case P2X.MODE_BINARY:
                this.pushBinary(first);
                break;
            case P2X.MODE_POSTFIX:
                this.pushPostfix(first);
                break;
            case P2X.MODE_UNARY_BINARY:
                this.pushUnaryBinary(first);
                break;
            case P2X.MODE_LINE_COMMENT:
                this.pushIgnore(first);
                break;
            case P2X.MODE_BLOCK_COMMENT:
                this.pushIgnore(first);
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
                this.endList = [this.tokenInfo.getOpCode(P2X.TOKEN_EOF)]
            this.result = {}
            this.result.parser = this
            this.result.scanner = tlist.scanner
            this.input = tlist;

            var locInfo = function(tk) {
                return tlist.scanner.filename + ":" + tk.line + ":" + tk.col
            }

            var first

            var endFound = false;
            do {
                first = this.input.next()
                // console.log("Parser: next, text='" + first.text
                //             + "' code: " + this.tokenInfo.getOpCode(first)
                //             + ', mode: ' + this.tokenInfo.mode(first) + ', prec: ' + this.tokenInfo.prec(first))
                // console.dir(this.endList)
                if (typeof first == "undefined") {
                    console.error("Parser: unexpected end found, exiting")
                    // console.dir(first)
                    endFound = true
                } else if (this.endList.indexOf(this.tokenInfo.getOpCode(first)) > -1) {
                    // console.log("Parser: end found: "+ this.endList + ' ' + this.tokenInfo.getOpCode(first.token))
                    endFound = true
                } else if (first.token == P2X.TOKEN_EOF) {
                    console.error("Parser: unexpected end found")
                    for (k in this.endList) {
                        console.error("Parser: expecting " + this.endList[k])
                    }
                    endFound = true

                } else if (this.tokenInfo.mode(first) == P2X.MODE_LINE_COMMENT) {
                    var next, lncText = ''
                    while(true) {
                        next = this.input.next()
                        if (next.token == P2X.TOKEN_NEWLINE || next.token == P2X.TOKEN_EOF)
                            break
                        lncText += next.text
                    }
                    this.insertToken(first)
                    var inserted = first
                    if (next.token == P2X.TOKEN_NEWLINE) {
                        this.insertToken(next)
                    } else if (next.token == P2X.TOKEN_EOF) {
                        console.error("Unexpected end of input in line comment while searching for EOL (\\n)\n");
                        endFound = true
                        first = next
                    }
                    inserted.text += lncText

                } else if (this.tokenInfo.mode(first) == P2X.MODE_BLOCK_COMMENT && this.tokenInfo.isLParen(first)) {
                    var next, parent = this, pcommentEndList = this.tokenInfo.endList(first).map(function(k) {
                        return parent.tokenInfo.getOpCode(k.token, k.repr)
                    })
                    var ctext = first.text
                    while(true) {
                        next = this.input.next()
                        if (next.token == P2X.TOKEN_EOF)
                            break
                        ctext += next.text
                        if (pcommentEndList.indexOf(this.tokenInfo.getOpCode(next)) > -1)
                            break;
                        if (this.tokenInfo.mode(next) == P2X.MODE_BLOCK_COMMENT) {
                            console.log(locInfo(first) + ": Block comment " + next.text + " starts inside block comment")
                            console.log(locInfo(next) +  ": here, but nesting is not allowed")
                        }
                    }
                    this.insertToken(first)
                    var inserted = first
                    if (next.token == P2X.TOKEN_EOF) {
                        console.log("Unexpected end of input in block comment while searching for " << pcommentEndList << "\n")
                        endFound = true
                        first = next
                    }
                    inserted.fullText = ctext

                } else if (this.tokenInfo.isLParen(first)) {
                    var parser = P2X.Parser(this.tokenInfo)
                    var parent = this
                    parser.options = this.options
                    parser.endList = this.tokenInfo.endList(first).map(function(k) {
                        return parent.tokenInfo.getOpCode(k.token, k.repr)
                    })

                    var last = parser.parse(this.input)

                    if (this.tokenInfo.assoc(first) != P2X.ASSOC_RIGHT) {

                        if (typeof last == "undefined" || last.token == P2X.TOKEN_EOF) {
                            first.right = parser.root.right;
                        } else {
                            first.right = last;
                            last.left = parser.root.right;
                        }
                        this.insertToken(first)

                        this.leastMap.insert(this.tokenInfo.prec(first), first.right);
                    } else {

                        if (typeof last == "undefined" || last.token == P2X.TOKEN_EOF) {
                            this.insertToken(first);
                            first.right = parser.root.right;
                            this.leastMap.insert(this.tokenInfo.prec(first), first);
                        } else {
                            this.insertToken(last);
                            first.left = last.left;
                            last.left = first;
                            first.right = parser.root.right;
                            this.leastMap.insert(this.tokenInfo.prec(first), last);
                        }

                    }

                    if (parser.root.ignore) {
                        first.ignore = parser.root.ignore;
                    }

                    first = last;

                } else {
                    this.insertToken(first)
                }
            } while (! endFound && first)
            return first
        }
    }

    res.root = res.mkroot()
    res.leastMap.insert(res.tokenInfo.prec(res.root), res.root)

    return res
}

P2X.TreePrinterOptions = function(obj) {
    var res = {
        parseConf: false,
        scanConf: false,
        treewriterConf: false,
        caSteps: false,
        id: false,
        line: false,
        col: false,
        code: false,
        prec: true,
        mode: true,
        type: true,
        indent: true,
        newlineAsBr: true,
        merged: false,
        strict: 1,
        outputMode: 'x',
        caTextName: 'ca:text',
        ciNSPref: 'ca:',
        caNSPref: 'ca:',
        encoding: 'utf-8'
    }
    if (typeof obj == "object") {
        Object.keys(res).map(function(k){
            if (k in obj)
                res[k] = obj[k]
        })
    }
    if (res.outputMode == 'y') {
        res.caTextName = 'c:t'
        res.ciNSPref = 'ci:'
        res.caNSPref = 'c:'
    }
    return res
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
        opName1: function(t) {
            var tagname = 'op'
            if (this.tokenInfo.tagName(t))
                tagname = this.tokenInfo.tagName(t)
            else if (this.tokenInfo.isParen(t))
                tagname = 'paren'
            else if (t.token == P2X.TOKEN_FLOAT)
                tagname = 'float'
            else if (t.token == P2X.TOKEN_INTEGER)
                tagname = 'integer'
            else if (t.token == P2X.TOKEN_STRING)
                tagname = 'string'
            else if (t.token == P2X.TOKEN_IDENTIFIER && !this.tokenInfo.isOp(t))
                tagname = 'id'
            else if (t.token == P2X.TOKEN_ROOT)
                tagname = 'root'
            return tagname
        },
        opName2: function(t) {
            var tagname = 'op'
            if (this.tokenInfo.tagName(t))
                tagname = this.tokenInfo.tagName(t)
            else if (t.token == P2X.TOKEN_INTEGER)
                tagname = 'INT'
            else if (t.token == P2X.TOKEN_IDENTIFIER)
                tagname = 'ID'
            else
                tagname = P2X.ParserToken.names_index[t.token]
            return tagname
        },
        printParserConfig: function(t, indent, metainfo) {
            return indent + '<!--'+' printing the parser config as is not supported, please use TreePrinterPlus instead -->\n'
        },
        asxml: function(t, indent, metainfo) {
            var res = '';
            if (!indent) indent = ' '
            if (!metainfo) metainfo = {}
            if (metainfo && typeof metainfo != 'boolean') {
                if (this.options.outputMode == 'x')
                    res += "<code-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/' ca:version='1.0'>\n"
                else
                    res += "<code-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:c='http://johannes-willkomm.de/xml/code-xml/attributes/' xmlns:ci='http://johannes-willkomm.de/xml/code-xml/ignore'>\n"
                if (this.options.caSteps) {
                    res += indent + "<ca:steps/>\n"
                }
                if (this.options.scanConf && metainfo.scanner) {
                    // console.log('Scanner conf')
                    // console.dir(t.scanner.get())
                    res += metainfo.scanner.get().asxml()
                }
                if (this.options.parseConf && metainfo.parser) {
                    res += this.printParserConfig(t, indent, metainfo)
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
                var tagname = this.options.outputMode == 'x' ? this.opName1(t) : this.opName2(t)
                res += indent + '<' + tagname
                res += this.writeXMLLocAttrs(t)
                res += this.writeXMLTypeAttrs(t)
                res += '>';
                if (t.left || t.right || t.ignore)
                    res += '\n';
                if (t.left) {
                    res += this.asxml(t.left, indent + ' ', true);
                } else if (t.right) {
                    if (this.options.strict > 0)
                        res += indent + ' <null/>\n';
                }
                res += this.writeXMLTextElem(t, (t.left || t.right || t.ignore) ? indent + ' ' : '')
                if (t.ignore) {
                    res += this.writeIgnoreXML(t.ignore, indent + ' ');
                }
                res += this.asxml(t.right, indent + ' ', true);
                if (t.left || t.right || t.ignore)
                    res += indent
                res += '</' + tagname + '>\n';
            }
            if (metainfo && typeof metainfo != 'boolean') {
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
            if (t.text && t.token == P2X.TOKEN_IDENTIFIER && this.options.outputMode == 'x') {
                res += ' repr="' + t.text + '"'
            }
            var ttext
            if (this.options.type && this.options.outputMode == 'x') {
                if (t.token) {
                    if (t.token in P2X.ParserToken.names_index) {
                        ttext = P2X.ParserToken.names_index[t.token]
                    } else {
                        ttext = t.token
                    }
                } else
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
            var ttext = t.fullText || t.text || (t.token ? {text:''} : undefined) || t.toString() || string(t)
            if (typeof ttext == "object" && Object.keys(ttext).indexOf('text') > -1)
                ttext = ttext.text
            assert(typeof ttext == "string")
            if (ttext) {
                res = indent + P2X.TokenList.prototype.charasxml.apply(this.options, [ttext])
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
            var tagname = this.options.ciNSPref + (this.options.outputMode == 'x' ? "ignore" : P2X.ParserToken.names_index[t.token])
            res += indent + "<" + tagname;
            if (this.options.id)
                res += " id='" << t.id << "'";
            res += this.writeXMLLocAttrs(t);
            res += this.writeXMLTypeAttrs(t) + '>';
            if (this.options.outputMode == 'x')
                res += this.writeXMLTextElem(t, '')
            else
                res += t.text
            res += "</" + tagname + ">\n";
            return res
        }
    }
}

P2X.parseEvalExpr = function(text) {
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

P2X.parseJSON = P2X.parseEvalExpr

P2X.p2xj = function(input, p2xConf, result) {
    if (typeof result == "undefined") {
        result = {}
    }
    if (typeof p2xConf.rules != "object") {
        p2xConf.rules = P2X.parseJSON(p2xConf.rules)
    }
    if (typeof p2xConf.rules != "undefined") {
        if (p2xConf.debug) {
            result.uniConf = p2xConf.rules
        }
        p2xConf = P2X.UniConfParser().split(p2xConf)
        Object.keys(p2xConf).map(function(k){
            if (k != 'scanner' && k != 'parser' && k != 'rules') {
                p2xConf.scanner[k] = p2xConf[k]
                p2xConf.parser[k] = p2xConf[k]
            }
        })
    }
    var scanConf = p2xConf.scanner
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
        result.parseconf = parseConf
//        result.parseconf = parser.getconfig()
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

    var tpOptions = p2xConf.treewriter, defOpts = P2X.TreePrinterOptions();
    if (typeof tpOptions != "undefined") {
        if (typeof tpOptions != "object") {
            tpOptions = P2X.parseJSON(tpOptions)
        }
        if (typeof tpOptions == "undefined") {
            console.error('Failed to parse treewriter config')
            result.error = 'Failed to parse treewriter config'
        } else {
            Object.keys(tpOptions).map(function(k) {
                defOpts[k] = tpOptions[k]
            })
        }
    }
    var tp = P2X.TreePrinter(parser.tokenInfo, defOpts)

    tp.asxml(parser.root)

    result.xmlres = tp.asxml(parser.root)

    if (p2xConf.debug) {
        result.scanner = scanner
        result.parser = parser
        result.treewriter = tp
    }

    return result.xmlres
}

P2X.UniConfParser = function() {
    return {
        split: function(uniConf) {
            var scrule, prule, k, res, count = 0, scrules = {}, reid
            function getREID(rule) {
                if (rule.re in scrules) {
                    return scrules[rule.re]
                } else {
                    var reid = count
                    scrules[rule.re] = reid
                    scrule = {}
                    Object.keys(rule).map(function(x) {
                        if (x != 'assoc' && x != 'mode' && x != 'prec' && x != 'precU' && x != 'closingList' && x != 'isParen') {
                            scrule[x] = rule[x]
                        }
                    })
                    scrule.action = 1001 + reid
                    res.scanner.rules[reid] = scrule
                    ++count
                    return reid
                }
            }
            res = { scanner: { rules: [] }, parser: { rules: [] } }
            for (k=0; k < uniConf.rules.length; ++k) {
                rule = uniConf.rules[k]
                reid = getREID(rule)
                prule = { type: 1001 + reid }
                Object.keys(rule).map(function(x) {
                    if (x == 'closingList') {
                        cl = rule[x]
                        prule[x] = cl.map(function(clitem) {
                            reid = getREID(clitem)
                            var nit = { type: 1001 + reid }
                            return nit
                        })
                    } else if (x != 're') {
                        prule[x] = rule[x]
                    }
                })
                res.parser.rules[k] = prule
            }
            Object.keys(uniConf).map(function(x) {
                if (x == 'ignoreIgnore') {
                    res.parser[x] = uniConf[x]
                    res.scanner[x] = uniConf[x]
                } else {
                    res[x] = uniConf[x]
                }
            })
            // res.scanner = P2X.ScannerConfig(res.scanner)
            // res.parser = P2X.ParserConfig(res.parser)
            return res
        }
    }
}

P2X.scanner = P2X.JScanner('p2x1main')

if (typeof window == 'undefined') {
    P2X.importObject(P2X, exports)
}

console.log('scanner script loading')

var P2X = P2X || {};

P2X.importObject = function(obj, target) {
    for (k in obj) {
//        console.log('import: ' + k)
        target[k] = obj[k]
    }
}

var escapeRegExp = function(string){
    return string
        .replace(/[.*+?^${}()|[\]\\]/g, "\\$&")
        .replace(/\n/g, "\\n").replace(/\r/g, "\\r")
        .replace(/\t/g, "\\t").replace(/\v/g, "\\v")
}

var escapeXML = function(string){
  return string.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
}

if (typeof window == 'undefined') {
    
    var fs = require('fs')
    var mod_assert = require('assert')
    var pXML = require('./parseXml.js')

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
    return { token: tk, tokenName: ENUM.ParserToken.names_index[tk], text: text, index: index, line: line, col: col }
}

P2X.TokenList = function(list, producer) {
    console.log('TokenList created')
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
    return new P2X.TokenList([].concat(this.list, [eof]))
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
        return '<ca:text>' + s + '</ca:text>'
    }
}
P2X.TokenList.prototype.asxml = function() {
    var r = this.list
    var s = "<scan-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'>\n"
    if (this.scanner) {
        s += ' <input>' + escapeXML(this.scanner.input) + '</input>\n'
        s += this.scanner.get().asxml(' ')
    }
    for (k in r) {
        var t = r[k]
        if (t.token) {
            s += ' <token'
                + ' line="' + t.line + '"'
                + ' col="' + t.col + '"'
                + ' index="' + t.index + '"'
                + ' token="' + ENUM.ParserToken.names_index[t.token] + '">' + this.charasxml(t.text) + '</token>\n';
        } else {
            if (false) {
                s += '  <ca:ignored'
                    + ' line="' + t.line + '"'
                    + ' col="' + t.col + '"'
                    + ' index="' + t.index + '"'
                    + ' >' + this.charasxml(t.text) + '</ca:ignored>\n';
            }
        }
    }
    s += '</scan-xml>'
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
    rlist = []
    for (k in scanList.childNodes) {
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
            rlist.push(P2X.Token(ctoken.getAttribute('token'),
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
    res.asxml = function(indent) {
        if (typeof indent == 'undefined') indent = ' '
        res = indent + '<ca:scanner-config>\n'
        obj = {}
        for (k = 0; k < this.length; ++k) {
            var rule = this[k]
            res += indent + indent + '<ca:lexem'
            res += ' re="' + rule.re + '"'
            obj.act = rule.action
            // console.dir(typeof rule.action)
            // console.dir(obj.act.toString())
            // console.dir(obj.act())
            if (typeof rule.action == 'function') {
                if (rule.action.toString() == 'function () { return action }') {
                    var val = rule.action()
                    if (val in ENUM.ParserToken.names_index) {
                        res += ' action="' + ENUM.getParserTokenName(val) + '"'
                    } else {
                        res += ' action="' + val + '"'
                    }
                } else {
                    rule.action.toString()
                    res += ' action="' + rule.action.toString() + '"'
                }
            } else {
                var val = rule.action
                if (val in ENUM.ParserToken.names_index) {
                    res += ' action="' + ENUM.getParserTokenName(val) + '"'
                } else {
                    res += ' action="' + val + '"'
                }
            }
            // res += ' name="' + ENUM.getParserTokenName(rule[1].apply(ENUM.ParserToken, [])) + '"'
            res += '/>\n'
        }
        res += indent + '</ca:scanner-config>\n'
        return res
    }
    return res
}

P2X.JScanner = function(name) {
    console.log('JScanner created')
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
        add: function(re, action) {
            console.log(typeof re)
            console.log(typeof action)
            var fst_val, snd_val
            if (typeof re != 'object') {
                fst_val = RegExp(escapeRegExp(re))
            } else {
                fst_val = re
            }
            if (typeof action != 'function') {
                snd_val = function() { return action }
            } else {
                snd_val = action
            }
            this.action_dir[re.source] = [fst_val, snd_val]
            entry = [fst_val, snd_val]
            entry.count = 0
            this.actions.push(entry)
            return this
        },
        set: function(scconf) {
            this.actions = []
            this.action_dir = {}
            for (k in scconf) {
                this.add(scconf[k].re, scconf[k].action)
            }
            return this
        },
        get: function() {
            scconf = Array(this.actions.length)
            for (k in this.actions) {
                scconf[k] = {re: this.actions[k][0], action: this.actions[k][1]}
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
            return this
        },
        lex: function() {
            var re_nl = /\r/
            var matches = []
            var scanner = this
            var sinp = scanner.input.substr(this.yyindex)
            if (!sinp) return null
            var lengths = []
            var starts = []
            var results = this.actions.map(function (x) {
                var sr = x[0].exec(sinp)
                if (sr) {
                    lengths.push(sr[0].length)
                    starts.push(sr.index)
                } else {
                    lengths.push(0)
                    starts.push(undefined)
                }
//                console.log(sr)
                return sr
            })
            var max = -Infinity, maxk = -1;

            console.log(results)

            var min = arrayMin(starts);

            console.log(starts)
            console.log(lengths)

            console.log(min)

            var lengths = []
            var atStart = results.map(function (x) {
                if (x && x.index == min[0]) {
                    lengths.push(x.length)
                    return x
                } else {
                    lengths.push(0)
                    return null
                }
            })

            max = arrayMax(lengths);
            var first = max[1]

            console.log(max)
            console.log(atStart)
            console.log('first: ' + first)

            hit = atStart[first];

            ++this.actions[first].count
            
            var tt = ENUM.ParserToken
            tt.act = this.actions[first][1];
            var self = this;

            this.yyignored = {}
            
            if (min[0] > 0) {
                var skipped = sinp.substring(0, min[0])
                this.yyignored = { text: skipped,
                           index: this.yyindex,
                           line: this.yyline, 
                           col: this.yycol,
                          }
                var lnlind = skipped.lastIndexOf('\n')
                var lcrind = skipped.lastIndexOf('\r')
                console.log('lnlind: "' + skipped + ' "' + lnlind)
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

            res = P2X.Token(tt.act(), atStart[first][0], self.yyindex, self.yyline, self.yycol, first)
            
            this.yytext = atStart[first][0]
            // this.yyindex = atStart[first].index + atStart[first][0].length
            this.yyindex += this.yytext.length

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
            var l = []
            var c
            this.alllist = []
            this.token = []
            this.ignored = []
            do {
                c = this.lex();
                if (c) {
                    if (this.yyignored.text) {
                        this.ignored.push(this.yyignored)
                        this.alllist.push(this.yyignored)
                    }
                    this.token.push(c)
                    this.alllist.push(c)
                    l.push(c)
                }
            } while (c)
            return new P2X.TokenList(l, this)
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

P2X.TokenProto = function(tk, repr, mode, assoc, prec, precU, isParen) {
    var res
    if (typeof tk == 'object')
        res = P2X.TokenProto(tk.token, tk.repr, tk.mode, tk.assoc, tk.prec, tk.precU, tk.isParen) 
    else {
        if (!tk in ENUM.ParserToken.names_index) {
            console.error('Value token ' + tk + ' must be in the set of allowed token: ')
            console.dir(ENUM.ParserToken.names_index)
            assert(false)
        }
        if (tk != TOKEN_IDENTIFIER)
            repr = ''
        res = {
            token: tk, repr: repr, mode: mode, assoc: assoc, prec: prec, precU: precU, isParen: isParen,
        }
    }

    res.asxml = function(indent) {
        if (!indent) indent = ' '
        var res = ''
        res += indent + '<ca:op'
        res += ' type="' + ENUM.getParserTokenName(this.token) + '"'
        if (this.repr)
            res += ' repr="' + this.repr + '"'
        if (typeof this.mode != 'undefined')
            res += ' mode="' + ENUM.getParserModeName(this.mode) + '"'
        if (typeof this.assoc != 'undefined')
            res += ' assoc="' + ENUM.getParserAssocName(this.assoc) + '"'
        if (typeof this.precedence != 'undefined')
            res += ' precedence="' + this.prec + '"'
        if (this.mode == MODE_UNARY_BINARY)
            res += ' unary-precedence="' + this.precU + '"'
        if (typeof this.isParen != 'undefined')
            res += ' isParen="' + this.isParen + '"'
        res += '/>\n'
        return res
    }

    return res
}

P2X.ParserConfig = function(x) {
    res = Array.apply({}, x);
    res.asxml = function(indent) {
        if (!indent) indent = ' '
        var res = ''
        res += indent + '<ca:token-types>\n'
        for (k = 0; k < this.length; ++k) {
            res += this[k].asxml(indent + ' ')
        }
        res += indent + '</ca:token-types>\n'
        return res
    }
    return res
}

P2X.TokenInfo = function() {
    var tokenClasses = {}
    tokenClasses[TOKEN_JUXTA] = P2X.TokenProto({
        token: TOKEN_JUXTA, repr: '', mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 1, precU: 1, isParen: false
    })
    tokenClasses[TOKEN_ROOT] = P2X.TokenProto({
        token: TOKEN_ROOT, repr: '', mode: MODE_UNARY, assoc: ASSOC_LEFT, prec: 0, precU: 0, isParen: false
    })
    return {
        tokenClasses: tokenClasses,
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
            return this.get(tl).mode
        },
        assoc: function (tl) {
            return this.get(tl).assoc
        },
        binary_prec: function (tl) {
            return this.get(tl).prec
        },
        unary_prec: function (tl) {
            return this.get(tl).precU
        },
        prec: function (tl) {
            return this.binary_prec(tl)
        },
        precUnary: function (tl) {
            return this.get(tl).precU
        },
        isParen: function (tl) {
            return this.get(tl).isParen
        },
        getOpCode: function (tl, repr) {
            var res
            if (typeof tl == 'object' && 'token' in tl) {
                res = this.getOpCode(tl.token, tl.repr)
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
            for (k in this.tokenClasses) {
                res[n++] = this.tokenClasses[k]
            }
            return P2X.ParserConfig(res)
        },
        setconfig: function (pc) {
            this.tokenClasses = {}
            for (k in pc) {
                this.insert(pc[k])
            }
            return this
        },
        insert: function (tokenProto) {
            if (tokenProto.token == TOKEN_IDENTIFIER && tokenProto.repr) {
                // this creates the new op code
                console.log('inserting token proto for named ID ' + tokenProto.repr + ': ' + this.getOpCode(tokenProto.repr))
                console.log(' prec ' + tokenProto.prec)
                this.tokenClasses[this.getOpCode(tokenProto.repr)] = tokenProto
            } else {
                this.tokenClasses[this.getOpCode(tokenProto.token)] = tokenProto
            }
            return this
        },
    }
}

P2X.Parser = function(tokenInfo) {
    return {
        root: null,
        input: [],
        endList: [tokenInfo.getOpCode(TOKEN_EOF)],
        tokenInfo: tokenInfo,
        mkroot: function() {
            return P2X.Token(TOKEN_ROOT, '', 0, 0, 0);
        },
        mkJuxta: function(t) {
            return P2X.Token(TOKEN_JUXTA, '', t.line, t.column, t.index);
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
            while (tmp.right
                   && ((this.tokenInfo.prec(tmp) < prec && this.tokenInfo.mode(tmp) != MODE_POSTFIX) || 
                       (this.tokenInfo.tokenTypeEqual(tmp, t) && assoc == ASSOC_RIGHT))) {
                parent = tmp;
                tmp = tmp.right;
            }
            if ((this.tokenInfo.prec(tmp) < prec && this.tokenInfo.mode(tmp) != MODE_POSTFIX) ||
                (this.tokenInfo.tokenTypeEqual(tmp, t) && assoc == ASSOC_RIGHT)) {
                assert(tmp.right == undefined);
                tmp.right = t;
            } else {
                console.log('pushBinary: ins+rotate ')
                console.dir(parent)
                console.dir(tmp)
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
        pushItem: function(t) {
            var rmop = this.getRMOp();
            console.log('pushItem: RM op: ' + rmop)
            console.dir(rmop)
            if (this.tokenInfo.mode(rmop) == MODE_POSTFIX || rmop.right) {
                op = this.mkJuxta(t);
                this.pushBinary(op);
                rmop2 = this.getRMOp();
                console.log('pushItem: RM op II: ' + rmop)
                console.dir(rmop)
                console.dir(rmop2)
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
        insertToken: function(first) {
            console.log('insertToken: t: ')
            console.dir(first)
            var firstMode = tokenInfo.mode(first)
            assert(not(firstMode & MODE_PAREN)); // MODE_PAREN bit is cleared
            assert(firstMode != 0); // mode is not 0
            assert(firstMode != MODE_PAREN); // mode is not MODE_PAREN
            switch(firstMode) {
            case MODE_ITEM:
                this.pushItem(first);
                break;
            case MODE_IGNORE:
                // this.pushIgnoreAsBefore(first);
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
                console.log("error: Parser: invalid mode " + firstMode + "\n");
                exit(1);
                break;
            }
            return this
        },
        parse: function(tlist) {
            this.root = this.mkroot()
            this.input = tlist;

            var first

            var endFound = false;
            do {
                first = this.input.next()
                console.log("Parser: next, code: " + tokenInfo.getOpCode(first)
                            + ', mode: ' + ENUM.getParserModeName(tokenInfo.mode(first)) + ', prec: ' + tokenInfo.prec(first))
                console.dir(first)
                if (this.endList.indexOf(this.tokenInfo.getOpCode(first)) > -1) {
                    console.log("Parser: end found: "+ this.endList + ' ' + this.tokenInfo.getOpCode(first.token))
                    endFound = true
                } else if (this.tokenInfo.isParen(first)) {
                    var parser = P2X.Parser(this.tokenInfo)
                    parser.endList = this.tokenInfo.endList(first)
                    var subTree = parser.parse(this.input)
                    
                    //parser.pushIgnoreAsBefore(last);

                    first.content = parser.root.right
                    
                    assert(first.left == 0)
                    assert(first.right == 0)
                    
                    this.insertToken(first)

                    // if (parser.root->ignore) {
                    //     pushIgnore(parser.root->ignore);
                    // }
                } else {
                    this.insertToken(first)
                }
            } while (! endFound && first)
            return first
        }
    }        
}

P2X.TreePrinter = function(tokenInfo) {
    return {
        name: 'testTreePrinter',
        tokenInfo: tokenInfo,
        asxml: function(t, indent) {
            if (!indent) indent = ' '
            res = '';
            if (t) {
                var tagname = 'op'
                if (t.token == TOKEN_FLOAT)
                    tagname = 'float'
                else if (t.token == TOKEN_INTEGER)
                    tagname = 'int'
                else if (t.token == TOKEN_IDENTIFIER && !this.tokenInfo.isOp(t))
                    tagname = 'id'
                else if (t.token == TOKEN_ROOT)
                    tagname = 'root'
                if (t.token == TOKEN_ROOT) {
                    res += '<code-xml>\n'
                    res += P2X.scanner.get().asxml(indent)
                    res += this.tokenInfo.getconfig().asxml(indent)
                }
                res += indent + '<' + tagname
                res += ' line="' + t.line + '"'
                res += ' col="' + t.col + '"'
                res += ' code="' + this.tokenInfo.getOpCode(t) + '"'
                if (t.text && t.token != TOKEN_NEWLINE && t.token != TOKEN_CRETURN)
                    res += ' repr="' + t.text + '"'
                res += ' type="' + ENUM.ParserToken.names_index[t.token] + '"'
                res += '>\n';
                if (t.left) {
                    res += this.asxml(t.left, indent + ' ');
                } else if (t.right) {
                    res += indent + ' <null/>\n';
                }
                if (t.text)
                    res += indent + ' ' + P2X.TokenList.prototype.charasxml.apply({}, [t.text]) + '\n'
                res += this.asxml(t.right, indent + ' ');
                res += indent + '</' + tagname + '>\n';
                if (t.token == TOKEN_ROOT) {
                    res += '</' + 'code-xml' + '>\n';
                }
            }
            return res
        }
    }
}

P2X.scanner = P2X.JScanner('p2x1main')

if (typeof window == 'undefined') {
    exports.JScanner = P2X.JScanner
    exports.ScannerConfig = P2X.ScannerConfig
    exports.scanner = P2X.scanner
    exports.TokenList = P2X.TokenList
    exports.TokenInfo = P2X.TokenInfo
    exports.TokenProto = P2X.TokenProto
    exports.Parser = P2X.Parser
    exports.TreePrinter = P2X.TreePrinter
    exports.importObject = P2X.importObject
    //exports.P2X = P2X
}

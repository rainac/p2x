var P2X = P2X || {}

P2X.importObject = function(obj, target) {
    for (var k in obj) {
//        console.log('import: ' + k)
        target[k] = obj[k]
    }
}

P2X.importObject(require('./scanner.js'), P2X)

if (typeof window == 'undefined') {
    var ENUM = {}
    ENUM.ParserMode = require('./modes.ncd.js')
    P2X.importObject(ENUM.ParserMode, ENUM)
    ENUM.ParserToken = require('./token.ncd.js')
    P2X.importObject(ENUM.ParserToken, ENUM)
    ENUM.ParserAssoc = require('./assoc.ncd.js')
    P2X.importObject(ENUM.ParserAssoc, ENUM)
    var pXML = require('./parse-xml.js')
}

var parseXml = pXML.parseXml

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

P2X.escapeRegExp = function(str){
    return str
        .replace(/[.*+\?^${}()|[\]\\]/g, "\\$&")
        .replace(/\n/g, "\\n").replace(/\r/g, "\\r")
        .replace(/\t/g, "\\t").replace(/\v/g, "\\v")
}

P2X.unescapeRegExp = function(str){
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

P2X.ScannerConfigOrig = P2X.ScannerConfig

P2X.ScannerConfigPlus = function(x) {

    res = P2X.ScannerConfigOrig(x)

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

    return res
}

P2X.ScannerConfig = P2X.ScannerConfigPlus

P2X.TokenProtoRW = function() {
    var res = {}
    res.asxml = function(obj, indent) {
        if (!indent) indent = ' '
        var res = ''
        res += indent + '<ca:op'
        res += ' type="'
        if (obj.token in ENUM.ParserToken.names_index) {
            res += ENUM.ParserToken.getName(obj.token)
        } else {
            res += obj.token
        }
        res += '"'
        if (obj.token == TOKEN_IDENTIFIER && obj.repr)
            res += ' repr="' + obj.repr + '"'
        if (typeof obj.mode != 'undefined') {
            res += ' mode="' + ENUM.ParserMode.getName(obj.mode) + '"'
        }
        if (obj.mode == MODE_UNARY_BINARY || obj.mode == MODE_BINARY)
            res += ' associativity="' + ENUM.ParserAssoc.getName(obj.assoc) + '"'
        if (obj.mode != MODE_ITEM && !obj.isParen)
            res += ' precedence="' + obj.prec + '"'
        if (obj.name)
            res += ' name="' + obj.name + '"'
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
        res += ' type: '
        if (obj.token in ENUM.ParserToken.names_index) {
            res += 'TOKEN_' + ENUM.ParserToken.getName(obj.token)
        } else {
            res += obj.token
        }
        if (obj.repr)
            res += ', repr: "' + obj.repr + '"'
        if (typeof obj.mode != 'undefined') {
            res += ', mode: MODE_' + ENUM.ParserMode.getName(obj.mode) + ''
        }
        if (obj.mode == MODE_UNARY_BINARY || obj.mode == MODE_BINARY)
            res += ', assoc: ASSOC_' + ENUM.ParserAssoc.getName(obj.assoc) + ''
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

P2X.tokenInfo2JSON = function(tt) {
    var pcrw = P2X.ParserConfigRW()
    return pcrw.asJSON(tt.getconfig())
}

P2X.parser2JSON = function(p) {
    return P2X.tokenInfo2JSON(p.tokenInfo)
}

P2X.tokenInfo2XML = function(tt) {
    var pcrw = P2X.ParserConfigRW()
    return pcrw.asxml(tt.getconfig())
}

P2X.TreePrinterPlus = function(constr_a, constr_b) {
    var res = P2X.TreePrinter(constr_a, constr_b)
    res.origasxml = res.asxml
    res.printParserConfig = function(t, indent, metainfo) {
        var pcwr = P2X.ParserConfigRW()
        return pcwr.asxml(metainfo.parser.getconfig(), indent)
    }
    res.asxml = function(a,b,c) {
        return this.origasxml(a,b,c)
    }
    return res
}

if (typeof window == 'undefined') {
    exports.escapeBS = P2X.escapeBS;
    exports.escapeBSQLines = P2X.escapeBSQLines;
    exports.TokenProtoRW = P2X.TokenProtoRW
    exports.ParserConfigRW = P2X.ParserConfigRW
    exports.tokenInfo2JSON = P2X.tokenInfo2JSON
    exports.tokenInfo2XML = P2X.tokenInfo2XML
    exports.parser2JSON = P2X.parser2JSON
    exports.TreePrinterPlus = P2X.TreePrinterPlus
    exports.ScannerConfig = P2X.ScannerConfig
}

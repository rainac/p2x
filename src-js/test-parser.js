if (typeof window == 'undefined') {

    var fs = require('fs')
    var pXML = require('./parse-xml.js')
    // console.log('pXML:')
    // console.dir(pXML)
    console.log('load scanner script')
    var P2X = require('./scanner.js')
    //var O3XML = require('o3-xml-fork')
    var MostXML = require('most-xml')
    var assert = require("assert")

    var ENUM = {}
    ENUM.ParserMode = require('./modes.ncd.js')
    ENUM.ParserToken = require('./token.ncd.js')
    ENUM.ParserAssoc = require('./assoc.ncd.js')

}

var scanner = P2X.scanner

P2X.importObject(ENUM.ParserMode, this)
P2X.importObject(ENUM.ParserToken, this)
P2X.importObject(ENUM.ParserAssoc, this)

// console.dir(P2X)
//console.dir(MostXML)

console.log('load scanner setup script')
var r2 = require('./scanner-setup.js');

scanner.str('a + b *\n\r 1.2e-122')

// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
var tl = scanner.lexall()
// console.log('Lexall Result: ')
// console.dir(tl)

console.log('Lexall XML: ')
var scanListXML = tl.asxml()
console.log(scanListXML)

var scanDoc = pXML.parseXml(scanListXML)
console.log('Original token list: ')
console.log(tl)

tl = new P2X.TokenList()
var tokenFromXML = tl.loadXML(scanListXML)
console.log('Token list reloaded from XML: ')
console.log(tokenFromXML)
console.log(tl.asxml())
console.log(tokenFromXML.asxml())
assert(tokenFromXML.asxml() == scanListXML)

var tt = P2X.TokenInfo()
tt.insert(P2X.TokenProto(TOKEN_DIV, '/', MODE_BINARY, ASSOC_LEFT, 100, 0, false))
tt.insert(P2X.TokenProto(TOKEN_MULT, '*', MODE_BINARY, ASSOC_LEFT, 100, 0, false))
tt.insert(P2X.TokenProto(TOKEN_PLUS, '+', MODE_BINARY, ASSOC_LEFT, 90, 0, false))
tt.insert(P2X.TokenProto(TOKEN_MINUS, '-', MODE_BINARY, ASSOC_LEFT, 90, 0, false))
tt.insert(P2X.TokenProto(TOKEN_EQUAL, '=', MODE_BINARY, ASSOC_RIGHT, 50, 0, false))
tt.insert(P2X.TokenProto(TOKEN_SPACE, '=', MODE_IGNORE))
tt.insert(P2X.TokenProto(TOKEN_NEWLINE, '=', MODE_IGNORE))
tt.insert(P2X.TokenProto(TOKEN_CRETURN, '=', MODE_IGNORE))

tt.insert(P2X.TokenProto(TOKEN_IDENTIFIER, 'plus', MODE_BINARY, ASSOC_LEFT, 90, 0, false))
tt.insert(P2X.TokenProto(TOKEN_IDENTIFIER, 'minus', MODE_BINARY, ASSOC_LEFT, 90, 0, false))
tt.insert(P2X.TokenProto(TOKEN_IDENTIFIER, 'plus', MODE_BINARY, ASSOC_LEFT, 91, 0, false))

console.log(tt.getconfig().asxml())

var p2xparser = P2X.Parser(tt)
//console.log(p2xparser)

tokenFromXML = tokenFromXML.mkeof()
p2xparser.parse(tokenFromXML)

console.log('Parse result: ')
console.dir(p2xparser.root)
tp = P2X.TreePrinter(tt)
console.log(tp.asxml(p2xparser.root))

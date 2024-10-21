if (typeof window == 'undefined') {

    var fs = require('fs')
    var assert = require('assert')
    var pXML = require('./parse-xml.js')
    // console.log('pXML:')
    // console.dir(pXML)
    console.log('load scanner script')
    var P2X = require('./scanner.js')
    require('./scanner-setup.js')
    //var O3XML = require('o3-xml-fork')
    var MostXML = require('most-xml')

    var ENUM = {}
    ENUM.ParserMode = require('./modes.ncd.js')
    ENUM.ParserToken = require('./token.ncd.js')
    ENUM.ParserAssoc = require('./assoc.ncd.js')

}

var scanner = P2X.scanner

P2X.importObject(ENUM.ParserMode, this)
P2X.importObject(ENUM.ParserToken, this)
P2X.importObject(ENUM.ParserAssoc, this)

var sc = [{ re: /[a-zA-Z\u0070-\uffff][a-zA-Z\u0070-\uffff0-9_]*/, action: TOKEN_IDENTIFIER },
          { re: /[0-9_]+/, action: TOKEN_INTEGER },
          { re: /(([0-9_]+\.[0-9_]*)|([0-9_]*\.[0-9_]+))([eEdD](\+|-)?[0-9]+)?/, action: function() { return TOKEN_FLOAT } },
          { re: '=', action: TOKEN_EQUAL },
          { re: '+', action: TOKEN_PLUS },
          { re: RegExp('\\*'), action: TOKEN_MULT },
         ]
scanner.set(sc)
scanner.str('a + b *\n\r 1.2e-122')

// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
var tl = scanner.lexall()
console.log(tl)

var scanListXML = tl.asxml()
console.log(scanListXML)

assert(tl.list.length == 5)
assert(tl.list[0].token == TOKEN_IDENTIFIER)
assert(tl.list[0].text == 'a')

assert(tl.list[1].token == TOKEN_PLUS)
assert(tl.list[1].text == '+')
assert(tl.list[1].index == 2)

assert(tl.list[4].token == TOKEN_FLOAT)
assert(tl.list[4].text == '1.2e-122')
assert(tl.list[4].index == 10)
assert(tl.list[4].line == 2)
assert(tl.list[4].col == 1)

var osc = Array.apply({}, sc)

sc.push({ re: RegExp('[\n\r ]+'), action: TOKEN_SPACE })
scanner.set(sc)
scanner.str('a + b *\n\r 1.2e-122')

// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
var tl = scanner.lexall()
console.log(tl)

var scanListXML = tl.asxml()
console.log(scanListXML)

assert(tl.list.length == 9)
assert(tl.list[0].token == TOKEN_IDENTIFIER)
assert(tl.list[0].text == 'a')

assert(tl.list[2].token == TOKEN_PLUS)
assert(tl.list[2].text == '+')
assert(tl.list[2].index == 2)

assert(tl.list[8].token == TOKEN_FLOAT)
assert(tl.list[8].text == '1.2e-122')
assert(tl.list[8].index == 10)
assert(tl.list[8].line == 2)
assert(tl.list[8].col == 1)


sc = Array.apply({}, osc)
sc.push({ re: RegExp(' +'), action: TOKEN_SPACE })
sc.push({ re: '\n', action: TOKEN_NEWLINE })
sc.push({ re: RegExp('\\r'), action: TOKEN_CRETURN })
scanner.set(sc)
scanner.str('a + b *\n\r 1.2e-122')

var tl = scanner.lexall()
console.log(tl)

var scanListXML = tl.asxml()
console.log(scanListXML)

assert(tl.list.length == 11)
assert(tl.list[0].token == TOKEN_IDENTIFIER)
assert(tl.list[0].text == 'a')

assert(tl.list[2].token == TOKEN_PLUS)
assert(tl.list[2].text == '+')
assert(tl.list[2].index == 2)

assert(tl.list[10].token == TOKEN_FLOAT)
assert(tl.list[10].text == '1.2e-122')
assert(tl.list[10].index == 10)
assert(tl.list[10].line == 2)
assert(tl.list[10].col == 1)

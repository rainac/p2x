
if (typeof window == 'undefined') {
    
    var fs = require('fs')
    var pXML = require('./parseXml.js')
    // console.log('pXML:')
    // console.dir(pXML)
    console.log('load scanner script')
    var P2X = require('./scanner.js')
    //var O3XML = require('o3-xml-fork')
    var MostXML = require('most-xml')
    var child_process = require('child_process')
    var POpts = require('./parse-opts.js')

    var ENUM = {}
    ENUM.ParserMode = require('./modes.ncd.js')
    ENUM.ParserToken = require('./token.ncd.js')
    ENUM.ParserAssoc = require('./assoc.ncd.js')

    var argv = global.process.argv

    var configFile = ''
    var options
    var optDefs = [
        { short: 'p', long: 'prec-list' }
    ]
    
    console.dir(argv)
    options = POpts.parseOptions(argv, optDefs)
}

function waitforexit(child, callback) {
    if (child.exited) {
        return child
    } else {
        setTimeout(waitforexit, 100, child, callback)
    }
}

if ('prec-list' in options) {
    configFile = options['prec-list'][0]

    var cmd = 'p2x -T -p ' + configFile
    console.log('run ' + cmd)
    console.dir(child_process)
    var cnfXML
    var child = child_process.exec(cmd, { stdio: 'inherit' }, function(errc, stdout, stderr) { cnfXML = stdout } )
    child.exited = false
    child.on('exit', function() {
        this.exited = true
        console.log('P2X exited')
        this.callback()
    })
    child.callback = function() {
        console.log(cnfXML)
    }
    console.dir(child)
    
//    system(cmd)
}

console.dir(options)

var scanner = P2X.scanner

P2X.importObject(ENUM.ParserMode, this)
P2X.importObject(ENUM.ParserToken, this)
P2X.importObject(ENUM.ParserAssoc, this)

// console.dir(P2X)
//console.dir(MostXML)

console.log('load scanner setup script')
var r2 = require('./scanner-setup.js');

console.log(scanner.get().asxml(' '))

scanner.str('a + b *\n\r 1.2e-122')

// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
// console.log(scanner.lex())
var tl = scanner.lexall()
// console.log(tl)

var scanListXML = tl.asxml()
console.log(scanListXML)


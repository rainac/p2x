
if (typeof window == 'undefined') {
    
    var fs = require('fs')
    // console.log('load scanner script')
    var P2X = require('./scanner.js')
    var P2XTools = require('./p2x-tools.js')
    P2X.importObject(P2XTools, P2X)
    //var O3XML = require('o3-xml-fork')
    var child_process = require('child_process')
    var POpts = require('./parse-opts.js')
    var events = require('events')

    var ENUM = {}
    ENUM.ParserMode = require('./modes.ncd.js')
    ENUM.ParserToken = require('./token.ncd.js')
    ENUM.ParserAssoc = require('./assoc.ncd.js')

    var argv = global.process.argv

    var configFile = ''
    var options
    var optDefs = [
        { short: 'p', long: 'prec-list' },
        { short: 's', long: 'scan-only' },
        { short: 'S', long: 'scanner-config' },
        { short: 'i', long: 'arguments' },
        { short: 'o', long: 'outfile' },
    ]
    
    options = POpts.parseOptions(argv, optDefs)

    var emitter = new events.EventEmitter()
    
    emitter.on('parserConfigOK', function(next) {
        // console.log('event parserConfigOK was triggered:')
        next()
    })
    
    emitter.on('scannerConfigOK', function(next) {
        // console.log('event scannerConfigOK was triggered:')
        next()
    })

    emitter.on('next', function(next, ev) {
        // console.log('event "next" was triggered by:')
        // console.dir(ev)
        next()
    })
    
    emitter.on('p2xerror', function(next, ev) {
        // console.log('event "p2xerror" was triggered by:')
        // console.dir(ev)
        next()
    })
    
}

readScannerConfig();

var scanner = P2X.JScanner()
var parser = P2X.Parser()

function readScannerConfig() {
    if ('scanner-config' in options) {
        var configFile = options['scanner-config'][0]
        fs.readFile(configFile, function(err, data) {
            if (err) throw(err)
            var jsdata = loadScannerConfig(data)
            var outfile = options['outfile']
            if (outfile && outfile[0] != '-') {
                fs.writeFile(outfile[0], jsdata, function(err, data) {
                    if (err) throw(err)
                    console.log("Wrote JSON scanner to " + outfile[0])
                })
            } else {
                console.log(jsdata)
            }
        })
    } else {
        console.error('Scanner config file must be given');
    }
}

function loadScannerConfig(cnfScanXML) {
    var sconf = P2X.ScannerConfig().loadXML(cnfScanXML)
    scanner.set(sconf)
    // console.log('Scanner config loaded:')
    // console.log(scanner.get().asxml())
    // console.log(scanner.actions)
    return scanner.get().asjson()
}

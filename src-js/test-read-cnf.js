
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
    ]
    
    console.dir(argv)
    options = POpts.parseOptions(argv, optDefs)
    console.dir(options)

    var emitter = new events.EventEmitter()
    
    emitter.on('parserConfigOK', function(next) {
        console.log('event parserConfigOK was triggered:')
        next()
    })
    
    emitter.on('scannerConfigOK', function(next) {
        console.log('event scannerConfigOK was triggered:')
        next()
    })

    emitter.on('next', function(next, ev) {
        console.log('event "next" was triggered by:')
        console.dir(ev)
        next()
    })
    
    emitter.on('p2xerror', function(next, ev) {
        console.log('event "p2xerror" was triggered by:')
        console.dir(ev)
        next()
    })
    
}

readParserConfigFile()

function readParserConfigFile() {
    if ('prec-list' in options) {
        configFile = options['prec-list'][0]
        
        var cmd = 'p2x -T -p ' + configFile
        console.log('run ' + cmd)
        var cnfXML
        // system(cmd)
        var child = child_process.exec(cmd, { stdio: 'inherit' },
                                       function(errc, stdout, stderr)
                                       {
                                           if (errc) {
                                               console.error('P2X exited with error:\n' + errc + stderr)
                                           } else {
                                               cnfXML = stdout;
                                               console.log('P2X exited2 errc::' + errc + ':: stdout::' + stdout + '::')
                                               parseConfig(cnfXML)
                                           }
                                       })
    } else {
        emitter.emit('noParserConfig', function(){});
        readScannerConfig();
    }
}

console.log('in script')

var scanner = P2X.JScanner()
var parser = P2X.Parser()

function parseConfig(cnfXML) {
    var pcrw = P2X.ParserConfigRW()
    var pc = pcrw.loadXML(cnfXML)
    // console.dir(pc)
    // console.log(pcrw.asxml(pc))
    
    parser.setconfig(pc)
    console.log('Parser config loaded:')
    console.log(pcrw.asJSON(parser.getconfig()))
    console.log(pcrw.asxml(parser.getconfig()))
    emitter.emit('next', readScannerConfig, 'readScannerConfig');
}

function readScannerConfig() {
    if ('scanner-config' in options) {
        var configFile = options['scanner-config'][0]
        fs.readFile(configFile, function(err, data) {
            if (err) throw(err)
            loadScannerConfig(data)
        })
    } else {
        emitter.emit('next', readInput, 'readInput');
    }
}

function loadScannerConfig(cnfScanXML) {
    var sconf = P2X.ScannerConfig().loadXML(cnfScanXML)
    scanner.set(sconf)
    // console.log('Scanner config loaded:')
    // console.log(scanner.get().asxml())
    // console.log(scanner.actions)
    emitter.emit('next', readInput);
}

function readInput() {
    if ('arguments' in options) {
        var inFile = options['arguments'][0]
        fs.readFile(inFile, function(err, data) {
            if (err) throw(err)
            parseInput(data + '')
        })
    } else {
        emitter.emit('fail', function() {
            console.log('no Input file')
        });
    }
}

function parseInput(data) {
    scanner.str(data)
    var tl = scanner.lexall().mkeof()
    console.log('scanned token list:')
    console.dir(tl)
    console.log(tl.asxml())
    var res = parser.parse(tl)
    tpOptions = P2X.TreePrinterOptions();
    var tp = P2X.TreePrinter(parser.tokenInfo, tpOptions)
    console.log('result XML:')
    console.log(tp.asxml(parser.root))
}

console.log('in script2')


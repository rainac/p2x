
if (typeof window == 'undefined') {
    
    var fs = require('fs')
    // console.log('load scanner script')
    var P2X = require('./scanner.js')
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
        { short: 'o', long: 'outfile' },
        { short: 'c', long: 'include-config', flag: 1 },
    ]
    
    // console.dir(argv)
    options = POpts.parseOptions(argv, optDefs)
    // console.dir(options)

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
    
    emitter.on('fail', function(next, ev) {
        console.error('error: p2xjs: ')
        next()
    })
    
}

var parser = P2X.Parser()

readParserConfigFile()

function readParserConfigFile() {
    if ('prec-list' in options) {
        configFile = options['prec-list'][0]
        importParserConfig({file: configFile}, function(pc) {
            parser.setconfig(pc);
            emitter.emit('next', readScannerConfig, 'readParserConfigFile');
        })
    } else {
        emitter.emit('noParserConfig', function(){});
        emitter.emit('next', readScannerConfig, 'readParserConfigFile');
    }
}

function importParserConfig(pconf, callback) {
    if (typeof pconf == "string") {
        interpretParserConfigText(pconf, callback)
    } else if (pconf.file) {
        fs.readFile(pconf.file, function(err, data) {
            if (err) throw(err)
            interpretParserConfigText(''+data, callback)
        })
    } else {
        callback(pconf)
    }
}

function interpretParserConfigText(pconf, callback) {
//    console.log(pconf)
    if (pconf[0] == '<') {
        pcrw = P2X.ParserConfigRW()
        callback(pcrw.loadXML(pconf))
    } else if (pconf[0] == '{' || pconf[0] == '[') {
        callback(P2X.parseJSON(pconf))
    } else {

        var cnfFileName = 'tmppconf.txt'

        fs.writeFile(cnfFileName, pconf, function(err) {
            if (err) throw(err)

            var cmd = 'p2x -T -p ' + cnfFileName
            // console.log('run ' + cmd)
            var cnfXML
            // system(cmd)
            var child = child_process.exec(cmd, { stdio: 'inherit' },
                                           function(errc, stdout, stderr)
                                           {
                                               if (errc) {
                                                   console.error('P2X exited with error:\n' + errc + stderr)
                                               } else {
                                                   cnfXML = stdout;
                                                   // console.log('P2X exited2 errc::' + errc + ':: stdout::' + stdout + '::')
                                                   interpretParserConfigText(cnfXML, callback)
                                               }
                                           })
        })
        
    }
}

var scanner = P2X.JScanner()

function readScannerConfig() {
    if ('scanner-config' in options) {
        var configFile = options['scanner-config'][0]
        fs.readFile(configFile, function(err, data) {
            if (err) throw(err)
            loadScannerConfig('' + data)
        })
    } else {
        emitter.emit('next', readInput, 'readInput');
    }
}

function loadScannerConfig(cnfScanXML) {
    //    var sconf = P2X.ScannerConfig().loadXML(cnfScanXML)
    var sconf
    if (cnfScanXML[0] == '<') {
        sconf = P2X.ScannerConfig().loadXML(cnfScanXML)
    } else {
        sconf = P2X.parseJSON(cnfScanXML)
    }
//    console.log('Scanner config parsed:' + sconf)
    scanner.set(sconf)
    // console.log('Scanner config loaded:')
    // console.log(scanner.get().asxml())
    // console.log(scanner.actions)
    emitter.emit('next', readInput);
}

function readInput() {
    if (options.arguments.length > 0) {
        var inFile = options['arguments'][0]
        fs.readFile(inFile, function(err, data) {
            if (err) throw(err)
            parseInput(data + '')
        })
    } else {
        emitter.emit('fail', function() {
            console.error('no Input file')
        });
    }
}

function parseInput(data) {
    scanner.str(data)
    var tl = scanner.lexall().mkeof()
    // console.log('scanned token list:')
    // console.dir(tl)
    // console.log(tl.asxml())
    var res = parser.parse(tl)
    tpOptions = P2X.TreePrinterOptions();
    if ('include-config' in options) {
        tpOptions.scanConf = tpOptions.parseConf = tpOptions.treewriterConf = true
    }
    var tp = P2X.TreePrinter(parser.tokenInfo, tpOptions)
    // console.log('result XML:')
    var res = tp.asxml(parser.root)

    if ('outfile' in options) {
        var outfile = options['outfile'][0];
        fs.writeFile(outfile, res)
    } else {
        console.log(res)
    }
}

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
        { short: 'S', long: 'scanner-config' },
        { short: 'o', long: 'outfile' },
    ]

    options = POpts.parseOptions(argv, optDefs)

    var emitter = P2X.emitter

    var conf = {}

    emitter.on('start', function(next) {
        if (next) { next() }
        P2X.readScannerConfigFile(options['scanner-config'][0])
    })

    emitter.on('scannerConfigOK', function(next, scanConf) {
        console.log('event scannerConfigOK was triggered:')
        if (next) { next() }
        conf.sConf = scanConf
        P2X.readParserConfigFile(options['prec-list'][0])
    })

    emitter.on('parserConfigOK', function(next, pconf) {
        // console.log('event parserConfigOK was triggered:')
        if (next) { next() }
        conf.pConf = pconf
        var uni = unifyConfigs(conf)
        writeOutput(uni)
    })

    emitter.on('p2xerror', function(next, ev) {
        console.log('event "p2xerror" was triggered by:')
        console.dir(ev)
        next()
    })

}

emitter.emit('start', function() {})

function unifyConfigs(conf) {

    var sctxt = P2X.JScanner().set(conf.sConf).get()
    console.log('sconf', P2X.JScanner().set(conf.sConf))
    console.log('sconf', P2X.JScanner().set(conf.sConf).get())
    var pctxt = P2X.Parser().setconfig(conf.pConf).tokenInfo.getconfig()

    var uni = []

    var mergePConfItem = function(pConfItem) {
        var res
        var aname = P2X.ParserToken.names_index[pConfItem.token]
        var action = P2X.ParserToken.get(pConfItem.token)
        console.log('matching ' + pConfItem.token + ' => ' + action + ' (' + aname + ')')
        var found = false
        for (var k in sctxt) {
            if (! ('action' in sctxt[k])) continue
            if (action == P2X.ParserToken.getValue(sctxt[k].action)) {
                var merged = pConfItem
                merged.re = sctxt[k].re
                delete merged.token
                merged.assoc = P2X.ParserAssoc.getName(merged.assoc)
                merged.mode = P2X.ParserMode.getName(merged.mode)
                found = true
                res = merged
            }
        }
        if (! found) {
            console.warn('Failed to match scan rule')
            console.warn(sctxt[k])
        }
        return res
    }

    for (var j in pctxt) {
        var merged = mergePConfItem(pctxt[j])
        if (merged) {
            uni.push(merged)
            console.log('merged', merged)
            if (merged.closingList && merged.closingList.length > 0) {
                for (var k in merged.closingList) {
                    merged.closingList[k] = mergePConfItem(merged.closingList[k])
                }
            }
        }
    }

    return uni
}

function writeOutput(uni) {
    var ofname = options['outfile'][0]
    console.log('uni', uni)
    console.dir(uni)
    var odata = JSON.stringify(uni, null, 1)
    fs.writeFile(ofname, odata, function(err, data) {
        if (err) throw(err)
        console.log('Wrote to ' + ofname)
    })
}

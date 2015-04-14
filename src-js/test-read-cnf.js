
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
}

console.log('in script')

var parseConfig = function(cnfXML) {
    console.log('parseConfig : ' + cnfXML)
}

console.log('in script2')

console.dir(options)


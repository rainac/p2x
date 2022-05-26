var optDefs = [
    { short: 'i', long: 'infile' },
    { short: 'o', long: 'outfile' },
]

var POpts = require('./parse-opts.js')
var options = POpts.parseOptions(global.process.argv, optDefs)

var compressor = require('node-minify');

var infiles = options['arguments']
var outfile = options['outfile'][0]

compressor.minify({
    compressor: 'uglifyjs',
    input: infiles,
    output: outfile,
    callback: function(err, min) {}
});

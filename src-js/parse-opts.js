
var POpts = POpts || {}

POpts.parseOptions = function(argv, optDefs) {
    
    var opt, optarg, options = { arguments: [] }, k
    
    for (k in optDefs) {
        var od = optDefs[k]
        if (! ('action' in Object.keys(od))) {
            od.action = function(data) {
                if (this.long in options) {
                    options[this.long].push(data)
                } else {
                    options[this.long] = [data]
                }
            }
        }
    }
    
    var matchShortOption = function(optc) {
        return argv[k].substring(0, 2) == ('-' + optc)
    }
    var matchLongOption = function(optname) {
        return argv[k].substring(0, 2 + optname.length) == ('--' + optname)
    }
    
    var procShortOption = function(optDef) {
        opt = argv[k].substring(1, 2)
        if (argv[k].length > 2) {
            optarg = argv[k].substring(2)
        } else {
            ++k
            optarg = argv[k]
        }
        optDef.action(optarg)
    }
    
    var procLongOption = function(optDef) {
        opt = argv[k].substring(2, 2+optDef.long.length)
        if (argv[k].length > 3+optDef.long.length) {
            optarg = argv[k].substring(3+optDef.long.length)
        } else {
            ++k
            optarg = argv[k]
        }
        optDef.action(optarg)
    }
    
    for (k = 2; k < argv.length; ++k) {
        var procd = false
        for (l = 0; l < optDefs.length; ++l) {
            optDef = optDefs[l]
            if (matchShortOption(optDef.short)) {
                procShortOption(optDef)
                procd = true
                break
            } else if (matchLongOption(optDef.long)) {
                procLongOption(optDef)
                procd = true
                break
            }
        }
        if (!procd) {
            options.arguments[options.arguments.length] = argv[k]
        }
    }

    return options
}

exports.parseOptions = POpts.parseOptions

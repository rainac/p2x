function runP2X() {
    var p2xConf = P2X.parseJSON(document.forms.p2x.uniconf.value)
    p2xConf.treewriter = P2X.parseJSON(document.forms.p2x.twconf.value)
    p2xConf.debug = true

    var res = {}
    var input = document.forms.p2x.input.value
    var output = P2X.p2xj(input, p2xConf, res)

    var fb = document.getElementById('input_feedback')
    fb.innerHTML = '<kbd>' + input + '</kbd>'

    var fb = document.getElementById('uniconf_feedback')
    fb.innerHTML = '<kbd>' + res.uniConf.toSource() + '</kbd>'

    var fb = document.getElementById('scanconf_feedback')
    fb.innerHTML = '<kbd>' + res.scanner.get().toSource() + '</kbd>'

    var fb = document.getElementById('parseconf_feedback')
    var pcrw = P2X.ParserConfigRW()
    fb.innerHTML = '<kbd>' + pcrw.asJSON(res.parser.getconfig()) + '</kbd>'

    var fb = document.getElementById('twconf_feedback')
    var twcnf = res.treewriter.options.toSource()
    fb.innerHTML = '<kbd>' + twcnf + '</kbd>'

    var tl = res.tokenlist
    var fb = document.getElementById('scan_feedback')
    tl.scanner = null
    fb.innerHTML = '<kbd>' + tl.asxml() + '</kbd>'
    fb.children[0].scrollTop = fb.children[0].scrollHeight

    document.forms.p2x.output.value = output
}
function gete(e, k) {
    var res
    Object.keys(e.children).map(function (x, i) {
        if (e.children[x].nodeType == e.ELEMENT) {
            if (k == 0) {
                res = i;
            }
            k = k - 1;
        }
    })
    return e.children[res]
}
function hide(e) {
    e.style.display = 'none';
    e.style.visibility = 'hidden';
}
function showb(e) {
    e.style.visibility = 'visible';
    e.style.display = 'block';
}
function jd(str) {
    var elem, cl
    if (str.contains('.')) {
        var p = str.indexOf('.')
        elem = str.substring(0, p);
        if (p == 0) {
            elem = '*';
        }
        cl = str.substring(p+1);
    }
    var tareas
    if (elem == '*') {
        res = document.getElementsByClassName(cl)
    } else {
        tareas = document.getElementsByTagName(elem)
        res = []
        tareas.map(function(k){
            if (k.classList.contains(cl)) {
                res[rex.length] = k
            }
        })
    }
    return res
}
function init() {
    var tareas = jd('.interact')
    for (var k=0; k<tareas.length;++k) {
        var x = tareas[k]
        var handler = function() {
            runP2X();
        }
        x.onkeyup = x.onchange = handler

        var e = x.parentNode
        var c1 = e.children[1]
        var c2 = e.children[2]
        c1.style.height = '10em'
        c2.style.height = '10em'
        hide(c1)

        var f = function(e, ev) {
            e = e.parentNode
            var c1 = e.children[1]
            var c2 = e.children[2]
            hide(c2)
            showb(c1)
        }
        x.onmouseover = function(ev) { f(this, ev); }
        var g = function(e, ev) {
            e = e.parentNode
            var c1 = e.children[1]
            var c2 = e.children[2]
            showb(c2)
            hide(c1)
        }
        x.onmouseout = function(ev) { g(this, ev); }
    }
}
function monmouse(x, ev) {
}


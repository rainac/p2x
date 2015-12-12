
var opxCreateRequest = function() {
    var req = null;
    try{
        req = new XMLHttpRequest();
    }
    catch (ms){
        try{
            req = new ActiveXObject("Msxml2.XMLHTTP");
        }
        catch (nonms){
            try{
                req = new ActiveXObject("Microsoft.XMLHTTP");
            }
            catch (failed){
                req = null;
            }
        }
    }
    if (req == null)
        alert("Error creating request object!");
    return req;
}


var opxReadyStateChangeData = function(x, d) {
    //    alert('opxReadyStateChange: ' + d.opxRequest.readyState);
    var data
    switch(d.opxRequest.readyState) {
    case 4:
        if (d.opxRequest.status != 200) {
            alert("Error: HTTP Status: " + d.opxRequest.status);
        } else {
            data = d.opxRequest.responseText;
            if (d.callback) {
                d.callback(data)
            }
        }
        break;
    default:
        return false;
        break;
    }
}

var getdata = function(url, callback) {
    method = 'GET'

    if (typeof callback == 'undefined') {
        callback = this.callback
    }
    
    var d = {
        request: opxCreateRequest(),
        callback: callback
        //    dataTarget: target;
        //    displayTarget: frame;
    }
    d.request.onreadystatechange = function() { opxReadyStateChange(null, d); };
    d.request.open(method, url, true);

}

var require = function(url, callback) {


    var head = document.head;

    var script = document.createElement( 'script' );
    script.type = 'text/javascript';
    script.src = url;

    head.appendChild(script)

    if (callback) {
        callback()
    }
    
}

var dummyThis = {}

var p2x_baseurl = p2x_baseurl || ''

function map_cb(array, callback_item, callback_end, index) {
    if (!index) index = 0
    if (index >= array.length) {
        callback_end(array)
        return
    }
    callback_item(function(){ map_cb(array, callback_item, callback_end, index+1) }, array[index], index)
}

function cb_require(callback, name, index) {
    require.call(dummyThis, name, callback)
}


scripts = [
    p2x_baseurl + 'modes.ncd.js',
    p2x_baseurl + 'token.ncd.js',
    p2x_baseurl + 'assoc.ncd.js',
    p2x_baseurl + 'parse-xml.js',
    p2x_baseurl + 'hashmap.js',
    p2x_baseurl + 'scanner.js'
]

map_cb(scripts, function(a,b,c) {
    // console.log('load ' + b)
    cb_require(a,b,c)
},
       function() { console.log('P2X loaded')})

//console.dir(dummyThis)

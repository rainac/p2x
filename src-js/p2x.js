
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

var require = function(url) {


    var head = document.head;

    var script = document.createElement( 'script' );
    script.type = 'text/javascript';
    script.src = url;

    head.appendChild(script)

}

var dummyThis = {}

var p2x_baseurl = p2x_baseurl || ''

//console.log(require.call)
require.call(dummyThis, p2x_baseurl + 'modes.ncd.js')
require.call(dummyThis, p2x_baseurl + 'token.ncd.js')
require.call(dummyThis, p2x_baseurl + 'assoc.ncd.js')
require.call(dummyThis, p2x_baseurl + 'parse-xml.js')
require.call(dummyThis, p2x_baseurl + 'scanner.js')

//console.dir(dummyThis)

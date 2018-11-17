
var nodejs = false
if (typeof window == 'undefined') {
    var MostXML = require('most-xml')
    var window = global;
    var exports = exports || {}
    nodejs = true
}
var pXML = pXML || {}

pXML.parseXml = null

pXML.setup = function() {

    if (typeof window.ActiveXObject != "undefined" &&
        new window.ActiveXObject("Microsoft.XMLDOM")) {
        pXML.parseXml = function(xmlStr) {
            if (typeof xmlStr != 'string') {
                xmlStr = String(xmlStr)
            }
            var xmlDoc = new window.ActiveXObject("Microsoft.XMLDOM");
            xmlDoc.async = "false";
            xmlDoc.loadXML(xmlStr);
            return xmlDoc;
        }
    } else if (typeof window.DOMParser != "undefined") {
        pXML.parseXml = function(xmlStr) {
            if (typeof xmlStr != 'string') {
                xmlStr = String(xmlStr)
            }
            return ( new window.DOMParser() ).parseFromString(xmlStr, "text/xml");
        }
    } else if (MostXML.loadXML) {
        pXML.parseXml = function(xmlStr) {
            if (typeof xmlStr != 'string') {
                xmlStr = String(xmlStr)
            }
            var xmlDoc = MostXML.loadXML(xmlStr)
            return xmlDoc
        }
    } else {
        throw new Error("No XML parser found")
    }

}
pXML.setup()

if (nodejs) {
    exports.parseXml = pXML.parseXml
}

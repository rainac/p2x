
var fs = require('fs')

var eres = eval(fs.readFileSync('token.ncd.js')+'')

console.log(ENUM.ParserToken.DOUBLE_RIGHT_ARROW)

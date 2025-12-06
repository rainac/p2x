
var assert = require("assert")
var fs = require("fs")
var child_process = require('child_process')

    function allLinesEqual(str1, str2) {
        ln1 = str1.split('\n')
        ln2 = str2.split('\n')
        for (k= 0; k < ln1.length; ++k) {
            assert.equal(ln1[k], ln2[k])
        }
    }

describe('Array', function(){
  describe('#indexOf()', function(){
    it('should return -1 when the value is not present', function(){
      assert.equal(-1, [1,2,3].indexOf(5));
      assert.equal(-1, [1,2,3].indexOf(0));
    })
  })
})

var P2X = require("../scanner.js")
var P2XTools = require("../p2x-tools.js")
P2X.importObject(P2XTools, P2X)

var TPOptions = function() {
    var tpo = P2X.TreePrinterOptions()
    tpo.line = true;
    tpo.col = true;
    tpo.type = true;
    return tpo
}

describe('P2X.ScannerConfig', function(){
  describe('#construct()', function(){
    // it('should contain values in list given to it', function(){
    //     assert.equal([1, 2, 3], Array.apply({},P2X.ScannerConfig([1,2,3])));
    // })
    it('should have the correct length', function(){
        assert.equal([1, 2, 3].length, P2X.ScannerConfig([1,2,3]).length);
    })
    it('should be an Array', function(){
        assert.equal(true, Array.isArray(P2X.ScannerConfig([1,2,3])));
    })
  })
  describe('#loadXML()', function(){
      it('should return XML rule list', function(){
          var xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          var scConf = P2X.ScannerConfig().loadXML(xmlRes)
          // console.dir(scConf)
          assert.equal(xmlRes, scConf.asxml(''));
      })
      it('on invalid input, no rule entry should be created', function(){
          var xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><rea>abc</rea><actions>1023</actions></ca:lexem>\n'
              + '</ca:scanner>\n'
          var xmlResEmpty = '<ca:scanner>\n'
              + '</ca:scanner>\n'
          var scConf = P2X.ScannerConfig().loadXML(xmlRes)
          assert.equal(scConf.length, 0);
          assert.equal(xmlResEmpty, scConf.asxml(''));
      })
      it('should return XML rule list', function(){
          var xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>abc/</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc/</ca:re><ca:action>TOKEN_IDENTIFIER</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc/</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          scConf = P2X.ScannerConfig().loadXML(xmlRes)
          // console.log(xmlRes)
          // console.dir(scConf)
          var scanner = P2X.JScanner()
          // console.dir(scanner.set(scConf).get())
          // console.dir(scanner.actions)
          // console.log(scConf.asxml(''))
          assert.equal(xmlRes, scConf.asxml(''));
      })
      function setAndGetFromScanner(scConf) {
          // console.dir(scConf)
          var myScanner = P2X.JScanner();
          myScanner.set(scConf)
          // console.dir(myScanner.actions)
          // console.dir(myScanner.get())
          return myScanner.get()
      }
      function setAndGetFromScannerXML(inXml) {
          var scConf = P2X.ScannerConfig().loadXML(inXml)
          return setAndGetFromScanner(scConf).asxml('')
      }
      it('should return XML rule list', function(){
          var inXml = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>11</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          var xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_KEYW_FUNCTION</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          assert.equal(xmlRes, setAndGetFromScannerXML(inXml));
      })
      it('should return XML rule list', function(){
          var inXml = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>\\(</ca:re><ca:action>TOKEN_MULT</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\'</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\\\\</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\\)</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>&amp;</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          assert.equal(inXml, setAndGetFromScannerXML(inXml));
      })
      it('should return XML rule list', function(){
          var inXml = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>\\(</ca:re><ca:action>TOKEN_MULT</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\'</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\"</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\\\\</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\\)</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>&amp;</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          assert.equal(inXml, setAndGetFromScannerXML(inXml));
      })
      function otstr(obj) {
          return JSON.stringify(obj);
      }
      it('set_get should return the same config', function(){
          var scConf = [
              { re: 'abc', action: 49 },
              { re: '1+2*3?', action: 49 },
              ]
          assert.equal(otstr(scConf), otstr(setAndGetFromScanner(scConf)));
      })
      it('set_get should return the same config', function(){
          var scConf = [
              { re: 'abc', action: 49 },
              { re: '1+2*3?', action: 49 },
          ]
          // console.dir(scConf)
          // console.dir(setAndGetFromScanner(scConf).aslist())
          assert.deepEqual(scConf, setAndGetFromScanner(scConf).aslist());
      })
      it('set_get should return the same config', function(){
          var scConf = [
              { re: 'abc', action: 49 },
              { re: '1+2*3?', action: P2X.TOKEN_INTEGER },
          ]
          // console.dir(scConf)
          // console.dir(setAndGetFromScanner(scConf).aslist())
          assert.deepEqual(scConf, setAndGetFromScanner(scConf).aslist());
      })
      it('set_get should leave strings as is', function(){
          var scConf = [
              { re: 'abc', action: 'INTEGER' },
              { re: '1+2*3?', action: 'TOKEN_INTEGER' },
          ]
          // console.dir(scConf)
          // console.dir(setAndGetFromScanner(scConf).aslist())
          assert.deepEqual(scConf, setAndGetFromScanner(scConf).aslist());
      })
      it('internally, the scanner should use integers', function(){
          var scConf = [
              { re: 'abc', action: 'INTEGER' },
              { re: '1+2*3?', action: 'TOKEN_INTEGER' },
          ]
          var scanner = P2X.JScanner()
          scanner.set(scConf)
          // console.dir(scanner.actions)
          assert.equal(typeof (scanner.actions[0][1]), 'number');
          assert.equal(typeof (scanner.actions[1][1]), 'number');
      })
      it('set_get should return the same config', function(){
          var scConf = [
              { re: '\\(', action: 49 },
              { re: '\'', action: 48 },
              { re: '\\?', action: 48 },
              { re: '\\**', action: 48 },
              { re: '\\)', action: 48 },
              { re: '&', action: 47 }
              ]
          assert.equal(otstr(scConf), otstr(setAndGetFromScanner(scConf)));
      })
      it('set RE, get should return the RE source strings in config', function(){
          var scConf = [
              { re: /\(/, action: 149 },
              { re: /'/, action: 48 },
              { re: /\?/, action: 48 },
              { re: /\**/, action: 48 },
              { re: /\)/, action: 48 },
              { re: /&/, action: 47 }
          ]
          var patched = scConf.map(function(x) { return {re: x.re.source, action: x.action}})
          assert.equal(otstr(patched), otstr(setAndGetFromScanner(scConf)));
      })
      it('empty RE should be rejected', function(){
          var scConf = [
              { re: '', action: 48 },
              ]
          assert.equal('[]', otstr(setAndGetFromScanner(scConf)));
      })
      it('empty RE should be rejected', function(){
          var scConf = [
              { re: '', action: 48 },
              { re: 'abc', action: 120 },
              ]
          assert.equal('['+otstr(scConf[1])+']', otstr(setAndGetFromScanner(scConf)));
      })
      it('ScannerConfig can be serialized to JSON', function(){
          var scConf = [
              { re: '123', action: 1148 },
              { re: 'abc', action: 11120 },
          ]
          // console.dir(scConf)
          // console.dir(P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()))
          assert.deepEqual(scConf, P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()));
      })
      it('ScannerConfig can be serialized to JSON (also directly)', function(){
          var scConf = [
              { re: '\'', action: 1148 },
              { re: 'abc', action: 11120 },
          ]
          var scanner = P2X.JScanner()
          scanner.set(P2X.ScannerConfig(scConf))
          // console.dir(scConf)
          // console.dir(P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()))
          assert.deepEqual(scConf, P2X.parseJSON(scanner.asjson()));
      })
      it('ScannerConfig can be serialized to JSON (II)', function(){
          var scConf = [
              { re: '\'', action: 1148 },
              { re: 'abc', action: 11120 },
          ]
          // console.dir(scConf)
          // console.dir(P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()))
          assert.deepEqual(scConf, P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()));
      })
      it('ScannerConfig can be serialized to JSON (apos,bs char)', function(){
          var scConf = [
              { re: '\'\\\'', action: 1148 },
              { re: 'abc', action: 11120 },
          ]
          // console.dir(scConf)
          // console.dir(P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()))
          assert.deepEqual(scConf, P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()));
      })
      it('ScannerConfig can be serialized to JSON (object with rules)', function(){
          var scConf = { ignored: true, rules: [
              { re: '\'\\\'', action: 1148 },
              { re: 'abc', action: 11120 },
          ] }
           console.dir(scConf)
           console.dir(P2X.ScannerConfig(scConf))
           console.dir(P2X.ScannerConfig(scConf).asjson())
          assert.deepEqual(scConf, P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()));
      })
      it('ScannerConfig can be serialized to JSON (nl, tab char)', function(){
          var scConf = [
              { re: '\'\n\t', action: 1148 },
              { re: 'abc', action: 11120 },
          ]
          // console.dir(scConf)
          // console.dir(P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()))
          assert.deepEqual(scConf, P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()));
      })
      it('ScannerConfig can be serialized to JSON (token codes are returned as names)', function(){
          var scConf = [
              { re: '\'\n\t', action: 48 },
              { re: 'abc', action: 20 },
          ]
          var scConfR = [
              { re: '\'\n\t', action: 'MINUS' },
              { re: 'abc', action: 'KEYW_VAR' },
          ]
          // console.dir(scConf)
          // console.dir(P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()))
          assert.deepEqual(scConfR, P2X.parseJSON(P2X.ScannerConfig(scConf).asjson()));
      })
      it('ScannerConfig can be serialized to JSON (file input)', function(){
          var configFile = '../examples/configs/scanner-test1.json'
          var scanner = P2X.JScanner()
          // console.log('load file ' + configFile)
          var data = fs.readFileSync(configFile)
          scanner.set(P2X.parseJSON(data))
          // console.log('Scanner config loaded:')
          // console.log(scanner.get())
          // console.log(scanner.get().asjson())
          // console.log(P2X.parseJSON(scanner.get().asjson()))
          assert.deepEqual(scanner.get().aslist(), P2X.parseJSON(scanner.get().asjson()))
      })
      it('ScannerConfig can be serialized to JSON (scanner-c template input)', function(){
          var configFile = '../examples/configs/scanner-c.json'
          var scanner = P2X.JScanner()
          // console.log('load file ' + configFile)
          var data = fs.readFileSync(configFile)
          scanner.set(P2X.parseJSON(data))
          // console.log('Scanner config loaded:')
          // console.log(scanner.get())
          // console.log(scanner.get().asjson())
          // console.log(P2X.parseJSON(scanner.get().asjson()))
          assert.deepEqual(scanner.get().aslist(), P2X.parseJSON(scanner.get().asjson()))
      })
      it('ScannerConfig can be serialized to JSON (large case)', function(){
          var configFile = '../examples/configs/scanner-c.xml'
          var scanner = P2X.JScanner()
          // console.log('load file ' + configFile)
          var data = fs.readFileSync(configFile)
          var sconf = P2X.ScannerConfig().loadXML(data)
          // console.dir(sconf.aslist())
          // console.dir(scanner.set(sconf).get().aslist())
          assert.deepEqual(sconf.aslist(), scanner.set(sconf).get().aslist())
          scanner.set(sconf)
          // console.log('Scanner config loaded:')
          // console.log(scanner.get().asxml())
          // console.log(scanner.actions)
          // console.log(scanner.get().asjson())
          // console.dir(P2X.parseJSON(scanner.get().asjson()))
          assert.deepEqual(scanner.get().aslist(), P2X.parseJSON(scanner.get().asjson()))
      })
  })
  describe('#asxml()', function(){
    it('should return empty XML', function(){
        assert.equal('<ca:scanner>\n</ca:scanner>\n',
                     P2X.ScannerConfig([]).asxml(''));
    })
      it('should return XML rule list', function(){
          var xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          var scConf = [
              {re: /abc/, action: 1023},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_DIV_EQUAL</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          scConf = [
              {re: /abc/, action: 100},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>a+b*c?</ca:re><ca:action>TOKEN_DOUBLE_LESS_EQUAL</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          action = 112
          scConf = [
              {re: /a+b*c?/, action: P2X.TOKEN_DOUBLE_LESS_EQUAL},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>ab\\\\c\\b</ca:re><ca:action>function() { return action*2 }</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          action = 102
          scConf = [
              {re: /ab\\c\b/, action: function() { return action*2 }},
          ]
          // console.log(xmlRes)
          // console.log(P2X.ScannerConfig(scConf).asxml(''))
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner>\n'
              + '<ca:lexem><ca:re>a&lt;&gt;+b&lt;&amp;*c?</ca:re><ca:action>TOKEN_DOUBLE_LESS_EQUAL</ca:action></ca:lexem>\n'
              + '</ca:scanner>\n'
          action = 102
          scConf = [
              {re: /a<>+b<&*c?/, action: P2X.TOKEN_DOUBLE_LESS_EQUAL},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
  })
})


describe('P2X.ParserConfig', function(){
    describe('#construct()', function(){
    })
    describe('#asxml()', function(){
        var confA, confB
        it('should return XML rule list', function(){
            xmlRes = '<ca:scanner>\n'
                + '<ca:lexem><ca:re>/abc/</ca:re><ca:action>function () { return action*2 }</ca:action></ca:lexem>\n'
                + '</ca:scanner>\n'
            var tt = P2X.TokenInfo()
            var p1 = P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false)
            tt.insert(p1)

            var tprw = P2X.TokenProtoRW()
            var pcrw = P2X.ParserConfigRW()
            // console.log(p1)
            // console.log(tprw.asxml(p1, ''))
            confA = tt.getconfig()
            confB = pcrw.loadXML(pcrw.asxml(confA, ''))
            console.log(pcrw.asxml(confA, ''))
            console.log(pcrw.asxml(confB, ''))
            assert.equal(pcrw.asxml(confA, ''), pcrw.asxml(confB, ''));
            assert.deepEqual(confA, confB);
            // console.log('==: ' + (confA == confB))
        })
        it('should return XML rule list', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MULT, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_PLUS, '+', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 90, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MINUS, '-', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 90, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_EQUAL, '=', P2X.MODE_BINARY, P2X.ASSOC_RIGHT, 50, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MINUS, '=', P2X.MODE_UNARY_BINARY, P2X.ASSOC_RIGHT, 50, 110, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_L_PAREN, '=', P2X.MODE_POSTFIX, P2X.ASSOC_NONE, 50, 0, true, [P2X.TokenProto(P2X.TOKEN_R_PAREN)]))
            tt.insert(P2X.TokenProto(P2X.TOKEN_SPACE, '=', P2X.MODE_IGNORE))
            tt.insert(P2X.TokenProto(P2X.TOKEN_NEWLINE, '=', P2X.MODE_IGNORE))
            tt.insert(P2X.TokenProto(P2X.TOKEN_CRETURN, '=', P2X.MODE_IGNORE))
            tt.insert(P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            var pcrw = P2X.ParserConfigRW()
            confA = tt.getconfig()
            confB = pcrw.loadXML(pcrw.asxml(confA, ''))
            // console.log(pcrw.asxml(tt.getconfig(), ''))
            // console.log(pcrw.asxml(confA, ''))
            // console.log(pcrw.asxml(confB, ''))
            assert.equal(pcrw.asxml(confA, ''), pcrw.asxml(confB, ''));
        })
//         it('should return XML rule list', function(){
//             var pcrw = P2X.ParserConfigRW()
//             var confA = pcrw.loadXML(fs.readFileSync('test1.xml'))
//             var confB = pcrw.loadXML(pcrw.asxml(confA, ''))
// //            console.log(pcrw.asxml(confA, ''))
//             assert.equal(pcrw.asxml(confA, ''), pcrw.asxml(confB, ''));
//         })

        it('ParserConfig can be serialized to JSON', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MULT, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_PLUS, '+', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 90, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MINUS, '-', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 90, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_EQUAL, '=', P2X.MODE_BINARY, P2X.ASSOC_RIGHT, 50, 0, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MINUS, '=', P2X.MODE_UNARY_BINARY, P2X.ASSOC_RIGHT, 50, 110, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_L_PAREN, '=', P2X.MODE_POSTFIX, P2X.ASSOC_NONE, 50, 0, true, false, [P2X.TokenProto(P2X.TOKEN_R_PAREN)]))
            tt.insert(P2X.TokenProto(P2X.TOKEN_SPACE, '=', P2X.MODE_IGNORE))
            tt.insert(P2X.TokenProto(P2X.TOKEN_NEWLINE, '=', P2X.MODE_IGNORE))
            tt.insert(P2X.TokenProto(P2X.TOKEN_CRETURN, '=', P2X.MODE_IGNORE))
            tt.insert(P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            var pcrw = P2X.ParserConfigRW()
            confA = tt.getconfig()

            tt.setconfig(P2X.parseJSON(pcrw.asJSON(confA)))
            tt.normalize()
            confB = tt.getconfig()

            for (var k in confA) {
                assert.deepEqual(confA[k], confB[k]);
            }
      })

        it('ParserConfig can be serialized to JSON', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(1100, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1101, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1102, '=', P2X.MODE_IGNORE))
            confA = tt.getconfig()

            tt = P2X.TokenInfo()
            confB = tt.setconfig([])
            confB = tt.getconfig()

            defConfig = [
  {
    token: 27,
    repr: '',
    mode: 3,
    assoc: 0,
    prec: 1,
    precU: 2,
    isParen: false,
    isRParen: false,
    closingList: undefined,
    name: undefined,
    ignoreIfStray: false,
  },
  {
    token: 71,
    repr: '',
    mode: 5,
    assoc: 1,
    prec: 2,
    precU: 2,
    isParen: false,
    isRParen: false,
    closingList: undefined,
    name: undefined,
    ignoreIfStray: false,
  },
  {
    token: 119,
    repr: '',
    mode: 1,
    assoc: 0,
    prec: 2,
    precU: 2,
    isParen: false,
    isRParen: false,
    closingList: undefined,
    name: undefined,
    ignoreIfStray: false,
  }
            ]

            assert.deepEqual(confB, defConfig);
        })
        it('ParserConfig can be serialized to JSON', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(1100, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1101, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1102, '=', P2X.MODE_IGNORE))
            var pcrw = P2X.ParserConfigRW()
            confA = tt.getconfig()

            tt = P2X.TokenInfo()
            tt.setconfig(P2X.parseJSON(pcrw.asJSON(confA)))
            tt.normalize()
            confB = tt.getconfig()

            assert.deepEqual(confA, confB);
      })

        it('ParserConfig can be serialized to JSON (also directly)', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(1100, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1101, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1102, '=', P2X.MODE_IGNORE))
            confA = tt.getconfig()
            tt.setconfig(P2X.parseJSON(P2X.tokenInfo2JSON(tt)))
            tt.normalize()
            confB = tt.getconfig()
            assert.deepEqual(confA, confB);
      })

        it('ParserConfig can be serialized to JSON (also from the parser)', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(1100, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1101, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1102, '=', P2X.MODE_IGNORE))
            var parser = P2X.Parser();
            parser.setconfig(tt.getconfig())
            confA = tt.getconfig()
            tt.setconfig(P2X.parseJSON(P2X.parser2JSON(parser)))
            tt.normalize()
            confB = tt.getconfig()
            assert.deepEqual(confA, confB);
      })

        it('Parser mode can be a string', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(1100, '/', 'BINARY', P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1101, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(1102, '=', P2X.MODE_IGNORE))
            var pcrw = P2X.ParserConfigRW()
            confA = tt.getconfig()

            tt.setconfig(P2X.parseJSON(pcrw.asJSON(confA)))
            tt.normalize()
            confB = tt.getconfig()

            assert.deepEqual(confA, confB);
        })

        it('rules for ROOT and JUXTA are always present', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false),
                P2X.TokenProto(P2X.TOKEN_MULT, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false)
            ]
            var confA = tt.getconfig()
            tt.setconfig(confSet);
            var confB = tt.getconfig()
            var tokenInListA = confA.map(function(x) { return x.token })
            var tokenInListB = confB.map(function(x) { return x.token })
            for (var k in tokenInListA) {
                assert(tokenInListB.indexOf(tokenInListA[k]) > -1)
            }
        })
        it('rules may be given as arbitrary objects', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                { type: P2X.TOKEN_DIV, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 100 },
                { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 100 },
            ]
            tt.setconfig(confSet);
            var confA = tt.getconfig()
            var res = ' <ca:parser>\n'
+'  <ca:op type="ROOT" mode="UNARY" precedence="1"/>\n'
+'  <ca:op type="MULT" mode="BINARY" associativity="LEFT" precedence="100"/>\n'
+'  <ca:op type="DIV" mode="BINARY" associativity="LEFT" precedence="100"/>\n'
+'  <ca:op type="JUXTA" mode="BINARY" associativity="LEFT" precedence="2"/>\n'
+'  <ca:op type="IGNORE" mode="IGNORE" precedence="2"/>\n'
+' </ca:parser>\n'
            // console.log(tt.asxml())
            allLinesEqual(res, P2X.tokenInfo2XML(tt))
        })
        it('rules may be given as arbitrary objects', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                { token: 1100, mode: 'binary', assoc: 'left', prec: 100, name: "foo" },
                { token: 1200, mode: 'BINARY', assoc: 'left', prec: 100 },
            ]
            tt.setconfig(confSet);
            var confA = tt.getconfig()
            // console.log(tt.asxml())
            assert(P2X.tokenInfo2XML(tt).indexOf('type="1100"') > -1)
            assert(P2X.tokenInfo2XML(tt).indexOf('type="1200"') > -1)
            assert(P2X.tokenInfo2XML(tt).indexOf('name="foo"') > -1)
        })
        it('fields may be missing (default binary assoc is left)', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                { token: 1100, mode: 'binary', prec: 100, name: "foo" },
                { token: 1200, mode: 'unary', prec: 100, name: "foo" },
                { token: 1300, mode: 'unary_binary', prec: 100, name: "foo" },
                { token: 1400, mode: 'item', prec: 100, name: "foo" },
            ]
            tt.setconfig(confSet);
            var confA = tt.getconfig()
            // console.log(tt.asxml())
            assert(P2X.tokenInfo2XML(tt).match(/type="1100".*associativity="LEFT"/))
//            assert(tt.asxml().match(/type="1200".*associativity="NONE"/))
            assert(P2X.tokenInfo2XML(tt).match(/type="1300".*associativity="LEFT"/))
//            assert(tt.asxml().match(/type="1400".*associativity="NONE"/))
        })
        it('fields may be missing (default mode is item)', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                { token: 1100, name: "foo" },
            ]
            tt.setconfig(confSet);
            var confA = tt.getconfig()
            // console.log(tt.asxml())
            assert(P2X.tokenInfo2XML(tt).indexOf('mode="ITEM"') > -1)
        })
        it('undefined prec defaults to 2', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_TILDE, '~', P2X.MODE_UNARY, P2X.ASSOC_NONE, undefined, undefined, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_PLUS, '/', P2X.MODE_UNARY_BINARY, P2X.ASSOC_LEFT, undefined, undefined, false))
            tt.insert(P2X.TokenProto(P2X.TOKEN_MULT, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, undefined, undefined, false))
            assert.equal(tt.prec(P2X.TOKEN_TILDE), 2)
            assert.equal(tt.prec(P2X.TOKEN_PLUS), 2)
            assert.equal(tt.precUnary(P2X.TOKEN_PLUS), 2)
            assert.equal(tt.prec(P2X.TOKEN_MULT), 2)
        })
        it('undefined prec of item defaults to infinity', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_MULT, '*', P2X.MODE_ITEM, undefined, undefined, undefined, false))
            assert.equal(tt.prec(P2X.TOKEN_MULT), P2X.maxPrec)
        })
        it('undefined prec of item defaults to infinity', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_L_PAREN, '*', P2X.MODE_ITEM, undefined, undefined, undefined, true))
            assert.equal(tt.prec(P2X.TOKEN_L_PAREN), P2X.maxPrec)
        })
        it('for plain token, opcode == type', function(){
            var tt = P2X.TokenInfo()
            assert.equal(tt.getOpCode(P2X.TOKEN_TILDE), P2X.TOKEN_TILDE)
            tt.insert(P2X.TokenProto(P2X.TOKEN_TILDE, '~', P2X.MODE_UNARY, P2X.ASSOC_NONE, undefined, undefined, false))
            assert.equal(tt.getOpCode(P2X.TOKEN_TILDE), P2X.TOKEN_TILDE)
        })
        it('for plain token, opcode == type', function(){
            var tt = P2X.TokenInfo()
            assert.equal(tt.getOpCode(1111), 1111)
            tt.insert(P2X.TokenProto(1111, '~', P2X.MODE_UNARY, P2X.ASSOC_NONE, undefined, undefined, false))
            assert.equal(tt.getOpCode(1111), 1111)
        })
        it('ROOT prec is 1', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_TILDE, '~', P2X.MODE_UNARY, P2X.ASSOC_NONE, undefined, undefined, false))
            // console.log(tt.tokenClasses)
            // console.log(tt.get(P2X.TOKEN_ROOT))
            assert.equal(tt.prec(P2X.TOKEN_ROOT), 1)
        })
        it('ROOT prec is 1', function(){
            try {
                var tt = P2X.TokenInfo()
                tt.insert(P2X.TokenProto(P2X.TOKEN_ROOT, '/', P2X.MODE_UNARY, P2X.ASSOC_NONE, 100, undefined, false))
                tt.insert(P2X.TokenProto(P2X.TOKEN_TILDE, '~', P2X.MODE_UNARY, P2X.ASSOC_NONE, undefined, undefined, false))
                assert.equal(0, 1)
            } catch (ME) {
            }
        })
        it('ignore mode is ignored', function(){
            var tt = P2X.TokenInfo()
            assert.equal(tt.mode(P2X.TOKEN_IGNORE), P2X.MODE_IGNORE)
        })
        it('ignore mode is ignored', function(){
            try {
                var tt = P2X.TokenInfo()
                tt.insert(P2X.TokenProto(P2X.TOKEN_IGNORE, '/', P2X.MODE_UNARY, P2X.ASSOC_NONE, 100, undefined, false))
                assert.equal(0, 1)
            } catch (ME) {
            }
        })
        it('precedence of unary_binary is determined dynamically', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(P2X.TOKEN_TILDE, '~', P2X.MODE_UNARY_BINARY, P2X.ASSOC_LEFT, 100, 200, false))
            assert.equal(tt.mode(P2X.TOKEN_TILDE), P2X.MODE_UNARY_BINARY)
            child = {token: P2X.TOKEN_INTEGER, text: '122'}
            node = { token: P2X.MODE_UNARY_BINARY, text: '-', left: child, right: child}
            assert.equal(tt.binary_prec({ token: P2X.TOKEN_TILDE, left: child, right: child}), 100)
            assert.equal(tt.unary_prec({ token: P2X.TOKEN_TILDE, left: child, right: child}), 200)
            assert.equal(tt.precedence({ token: P2X.TOKEN_TILDE, left: undefined, right: child}), 200)
            assert.equal(tt.precedence({ token: P2X.TOKEN_TILDE, left: child, right: child}), 100)
            assert.equal(tt.precedence({ token: P2X.TOKEN_TILDE, left: child, right: undefined}), 100)
        })
        it('precedence of paren is infite from outside', function(){
            var parseConf = {
                ignoreIgnore: true,
                rules: [
                    { type: P2X.TOKEN_L_PAREN, isParen: 1, closingList: [ { type: P2X.TOKEN_R_PAREN } ] },
                    { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                    { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                    { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
                ]
            }
            var parser = P2X.Parser()
            parser.setconfig(parseConf)
            var tokenp = {token: P2X.TOKEN_PLUS, left: {}, right: {}}
            var token = {token: P2X.TOKEN_L_PAREN, left: {}, right: {}}
            // console.dir(parser.tokenInfo)
            assert.equal(parser.tokenInfo.mode(tokenp), P2X.MODE_BINARY)
            assert.equal(parser.tokenInfo.mode(token), P2X.MODE_ITEM)
            assert.equal(parser.tokenInfo.isParen(token), true)
            assert.equal(parser.tokenInfo.precedence(token), P2X.maxPrec)
        })
        it('it should be possible to change the prec, assoc of JUXTA', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                P2X.TokenProto(P2X.TOKEN_JUXTA, '__JUXTA__', P2X.MODE_BINARY, P2X.ASSOC_RIGHT, 111, 0, false),
                P2X.TokenProto(P2X.TOKEN_DIV, '/', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false),
                P2X.TokenProto(P2X.TOKEN_MULT, '*', P2X.MODE_BINARY, P2X.ASSOC_LEFT, 100, 0, false)
            ]
            var confA = tt.getconfig()
            tt.setconfig(confSet);
            var confB = tt.getconfig()
            var tokenInListA = confA.map(function(x) { return x.token })
            var tokenInListB = confB.map(function(x) { return x.token })
            for (var k in tokenInListA) {
                assert(tokenInListB.indexOf(tokenInListA[k]) > -1)
            }
            assert.equal(tt.mode(P2X.TOKEN_JUXTA), P2X.MODE_BINARY)
            assert.equal(tt.prec(P2X.TOKEN_JUXTA), 111)
            assert.equal(tt.assoc(P2X.TOKEN_JUXTA), P2X.ASSOC_RIGHT)
        })
    })
})

describe('P2X.UniConfig', function(){
  describe('#split()', function(){
    it('can split a fused config into ScannerConfig and ParseConfig', function(){
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: 1001, mode: P2X.MODE_UNARY, prec: 100 },
                { type: 1002, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                { type: 1003, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                { type: 1004, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 }
            ]
        }
        var scanConf = {
            ignoreIgnore: true,
            rules: [
                {re: /abc/, action: 1001 },
                {re: /def/, action: 1002 },
                {re: /123/, action: 1003 },
                {re: /456/, action: 1004 }
            ]
        }
        var uniConf = {
            ignoreIgnore: true,
            rules: [
                { re: /abc/, mode: P2X.MODE_UNARY, prec: 100 },
                { re: /def/, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                { re: /123/, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                { re: /456/, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 }
            ]
        }
        var up = P2X.UniConfParser()
        var res = up.split(uniConf)
        assert.deepEqual(res.scanner, scanConf)
        assert.deepEqual(res.parser, parseConf)
    })
    it('Closing lists are handles as well', function(){
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: 1001, isParen: 1, closingList: [ { type: 1002 } ] },
            ]
        }
        var scanConf = {
            ignoreIgnore: true,
            rules: [
                {re: /\(/, action: 1001 },
                {re: /\)/, action: 1002 },
            ]
        }
        var uniConf = {
            ignoreIgnore: true,
            rules: [
                { re: /\(/, isParen: 1, closingList: [ { re: /\)/ } ] },
            ],
        }
        var up = P2X.UniConfParser()
        var res = up.split(uniConf)
        assert.deepEqual(res.scanner, scanConf)
        assert.deepEqual(res.parser, parseConf)
    })
    it('a treewriter options struct is preserved', function(){
        var twConf = TPOptions()
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: 1001, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
            ]
        }
        var scanConf = {
            ignoreIgnore: true,
            rules: [
                {re: 'abc', action: 1001 },
            ]
        }
        var uniConf = {
            ignoreIgnore: true,
            rules: [
                { re: 'abc', mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
            ],
            treewriter: TPOptions()
        }
        var up = P2X.UniConfParser()
        var res = up.split(uniConf)
        assert.deepEqual(res.scanner, scanConf)
        assert.deepEqual(res.parser, parseConf)
        assert.deepEqual(res.treewriter, twConf)
    })
    it('name fields are passed to the scanner and parser rules', function(){
        var twConf = TPOptions()
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: 1001, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300, name: 'testN' },
            ]
        }
        var scanConf = {
            ignoreIgnore: true,
            rules: [
                {re: 'abc', action: 1001, name: 'testN' },
            ]
        }
        var uniConf = {
            ignoreIgnore: true,
            rules: [
                { re: 'abc', mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300, name: 'testN' },
            ],
            treewriter: TPOptions()
        }
        var up = P2X.UniConfParser()
        var res = up.split(uniConf)
        assert.deepEqual(res.scanner, scanConf)
        assert.deepEqual(res.parser, parseConf)
        assert.deepEqual(res.treewriter, twConf)
    })
    it('a repeatedly occuring RE gets the same token ID', function(){
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: 1001, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
                { type: 1002, isParen: true, closingList: [ { type: 1003 }, { type: 1004 } ] },
                { type: 1005, isParen: true, closingList: [ { type: 1003 }, { type: 1001 } ] },
            ]
        }
        var scanConf = {
            ignoreIgnore: true,
            rules: [
                {re: 'abc', action: 1001 },
                {re: '\\(', action: 1002 },
                {re: '\\)', action: 1003 },
                {re: 'end', action: 1004 },
                {re: 'gg',  action: 1005 },
            ]
        }
        var uniConf = {
            ignoreIgnore: true,
            rules: [
                { re: 'abc', mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
                { re: '\\(', isParen: true, closingList: [ { re: '\\)' }, { re: 'end' } ] },
                { re: 'gg', isParen: true, closingList: [ { re: '\\)' }, { re: 'abc' } ] },
            ]
        }
        var up = P2X.UniConfParser()
        var res = up.split(uniConf)
        assert.deepEqual(res.scanner, scanConf)
        assert.deepEqual(res.parser, parseConf)
    }),
    it('any other fields are preserved', function(){
        var twConf = TPOptions()
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: 1001, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
            ]
        }
        var scanConf = {
            ignoreIgnore: true,
            rules: [
                {re: 'abc', action: 1001 },
            ]
        }
        var uniConf = {
            ignoreIgnore: true,
            rules: [
                { re: 'abc', mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
            ],
            treewriter: TPOptions(),
            debug: true,
            foo: 'bar',
        }
        var up = P2X.UniConfParser()
        var res = up.split(uniConf)
        assert.deepEqual(res.scanner, scanConf)
        assert.deepEqual(res.parser, parseConf)
        assert.deepEqual(res.treewriter, twConf)
        assert.equal(res.debug, uniConf.debug)
        assert.equal(res.foo, uniConf.foo)
        assert.equal(res.rules, uniConf.rules)
    })
    // it('uni rules may also be a string', function(){
    //     var twConf = TPOptions()
    //     var parseConf = {
    //         ignoreIgnore: true,
    //         rules: [
    //             { type: 1001, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },
    //         ]
    //     }
    //     var scanConf = {
    //         ignoreIgnore: true,
    //         rules: [
    //             {re: 'abc', action: 1001 },
    //         ]
    //     }
    //     var uniConf = "{\n"+
    //         "ignoreIgnore: true,\n"+
    //         "rules: [\n"+
    //         "    { re: 'abc', mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 300 },\n"+
    //         "],\n"+
    //         "treewriter: TPOptions(),\n"+
    //         "debug: true,\n"+
    //         "foo: 'bar',\n"+
    //     "}"
    //     var up = P2X.UniConfParser()
    //     var res = up.split(uniConf)
    //     assert.deepEqual(res.scanner, scanConf)
    //     assert.deepEqual(res.parser, parseConf)
    //     assert.deepEqual(res.treewriter, twConf)
    //     assert.equal(res.debug, true)
    //     assert.equal(res.foo, 'bar')
    // })
  })
})

describe('P2X.TokenList', function(){
    var xmlRes
    describe('#construct()', function(){
        var tl = new P2X.TokenList()
        it('should contain values in list given to it', function(){
//             assert.equal([1, 2, 3], Array.apply({},P2X.ScannerConfig([1,2,3])));
        }),
        it('should have field caTextName', function(){
            assert.equal(typeof tl.caTextName, 'string')
            console.dir(tl)
        })
    }),
    describe('#charasxml()', function(){
        var tl = new P2X.TokenList()
        var s = 'bopen'
        it('charasxml by default for s=' + s + ' prints this', function(){
            assert.equal(tl.charasxml(s), '<ca:text>bopen</ca:text>')
            console.dir(tl)
        })
    }),
    describe('#loadXML()', function(){
      it('should return XML rule list', function(){
          xmlRes = "<scan-xml xmlns='http://ai-and-it.de/xml/code-xml/' xmlns:ca='http://ai-and-it.de/xml/code-xml/attributes/'>\n"
              + '<token line="1" col="0" index="0" type="IDENTIFIER"><ca:text>a</ca:text></token>\n'
              + '<token line="1" col="1" index="1" type="SPACE"><ca:text> </ca:text></token>\n'
              + '<token line="1" col="2" index="2" type="PLUS"><ca:text>+</ca:text></token>\n'
              + '</scan-xml>\n'
          var tl = P2X.TokenList.prototype.loadXML(xmlRes)
          assert.equal(xmlRes, tl.asxml());
      })
      it('should return XML rule list', function(){
          xmlRes = "<scan-xml xmlns='http://ai-and-it.de/xml/code-xml/' xmlns:ca='http://ai-and-it.de/xml/code-xml/attributes/'>\n"
              + '<token line="1" col="0" index="0" type="IDENTIFIER"><ca:text>&lt;&gt;&amp;</ca:text></token>\n'
              + '<token line="1" col="1" index="1" type="NEWLINE"><ca:br/></token>\n'
              + '<token line="1" col="2" index="2" type="CRETURN"><ca:cr/></token>\n'
              + '</scan-xml>\n'
          var tl = P2X.TokenList.prototype.loadXML(xmlRes)
          assert.equal(xmlRes, tl.asxml());
      })
  })
})


describe('P2X.JScanner', function(){
  describe('#lexall()', function(){
    it('it should return a token list, XML output option', function() {
        var scanner = P2X.JScanner();
        var tl = scanner.lexall();
        var xmlRes = "<scan-xml xmlns='http://ai-and-it.de/xml/code-xml/' "
            + "xmlns:ca='http://ai-and-it.de/xml/code-xml/attributes/'>\n"
            + '<input></input>\n'
            + '<ca:scanner>\n'
            + '</ca:scanner>\n'
            + '</scan-xml>\n'
        assert.equal(tl.asxml(), xmlRes);
    })
    it('it should split a string into parts according to RE rules', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1', action: 1},
            { re: '3', action: 3},
            { re: 'abc', action: 111}
        ]
        scanner.set(scConf)
        var input = 'abc1abc3'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 4);

        assert.equal(tl.list[0].token, 111)
        assert.equal(tl.list[1].token, 1)
        assert.equal(tl.list[2].token, 111)
        assert.equal(tl.list[3].token, 3)

        assert.equal(tl.list[0].text, 'abc')
        assert.equal(tl.list[1].text, '1')
        assert.equal(tl.list[2].text, 'abc')
        assert.equal(tl.list[3].text, '3')
    })
    it('it should only return those token that match a rule', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1', action: 1},
            { re: '3', action: 3},
            { re: 'abc', action: 111}
        ]
        scanner.set(scConf)
        var input = 'rgtf 5 67'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 0);
    })
    it('it should only return those token that match a rule', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1', action: 1},
            { re: '3', action: 3},
            { re: 'abc', action: 111}
        ]
        scanner.set(scConf)
        var input = 'abc 12 \n ddds 3 dsa'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 3);

        assert.deepEqual(tl.list[0], { token: 111,
                                       tokenName: 'TILDE_EQUAL',
                                       text: 'abc',
                                       index: 0,
                                       line: 1,
                                       col: 0 })
        assert.deepEqual(tl.list[1], { token: 1,
                                       tokenName: 'KEYW_FOR',
                                       text: '1',
                                       index: 4,
                                       line: 1,
                                       col: 4 })
        assert.deepEqual(tl.list[2], { token: 3,
                                       tokenName: 'KEYW_WHILE',
                                       text: '3',
                                       index: 14,
                                       line: 2,
                                       col: 6 })
    })
      
    it('ignored token can be returned in request', function() {
        var scanner = P2X.JScanner()
        scConf = {
            ignored: true,
            rules: [
                { re: '1', action: 1},
                { re: '3', action: 3},
                { re: 'abc', action: 111}
            ]
        }
        scanner.set(scConf)
        var input = 'rgtf 5 67'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 1);
        assert.equal(tl.list[0].token, P2X.TOKEN_IGNORE)
        assert.equal(tl.list[0].text, 'rgtf 5 67')
    })
    it('ignored token can be returned in request', function() {
        var scanner = P2X.JScanner()
        scConf = {
            ignored: true,
            rules: [
                { re: '1', action: 1},
                { re: '3', action: 3},
                { re: 'abc', action: 111}
            ]
        }
        scanner.set(scConf)
        var input = 'abc 12 \n ddds 3 dsa'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 6);

        assert.equal(tl.list[1].token, P2X.TOKEN_IGNORE)
        assert.equal(tl.list[1].text, ' ')
        assert.equal(tl.list[3].token, P2X.TOKEN_IGNORE)
        assert.equal(tl.list[3].text, '2 \n ddds ')
        assert.equal(tl.list[5].token, P2X.TOKEN_IGNORE)
        assert.equal(tl.list[5].text, ' dsa')
        
        assert.deepEqual(tl.list[0], { token: 111,
                                       tokenName: 'TILDE_EQUAL',
                                       text: 'abc',
                                       index: 0,
                                       line: 1,
                                       col: 0 })
        assert.deepEqual(tl.list[2], { token: 1,
                                       tokenName: 'KEYW_FOR',
                                       text: '1',
                                       index: 4,
                                       line: 1,
                                       col: 4 })
        assert.deepEqual(tl.list[4], { token: 3,
                                       tokenName: 'KEYW_WHILE',
                                       text: '3',
                                       index: 14,
                                       line: 2,
                                       col: 6 })
    })

    it('it should match longest match first', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1',   action: 111},
            { re: '11',  action: 112},
            { re: '111', action: 113},
        ]
        scanner.set(scConf)
        var input = '111 11 1'
        scanner.str(input)
        var tl = scanner.lexall()

        assert.equal(tl.list.length, 3);

        assert.deepEqual(tl.list[0], { token: 113,
                                       tokenName: 'DOT_DIV_EQUAL',
                                       text: '111',
                                       index: 0,
                                       line: 1,
                                       col: 0 })
        assert.deepEqual(tl.list[1], { token: 112,
                                       tokenName: 'DOT_MULT_EQUAL',
                                       text: '11',
                                       index: 4,
                                       line: 1,
                                       col: 4 })
        assert.deepEqual(tl.list[2], { token: 111,
                                       tokenName: 'TILDE_EQUAL',
                                       text: '1',
                                       index: 7,
                                       line: 1,
                                       col: 7 })
    })

    it('with no rules, it should accept any input', function() {
        var scanner = P2X.JScanner()
        var input = 'abc 123 \n ddds'
        scanner.str(input)
        var tl = scanner.lexall()
    })
    it('with no rules, it should accept any input', function() {
        var scanner = P2X.JScanner()
        scConf = []
        scanner.set(scConf)
        var input = 'abc 123 \n ddds'
        scanner.str(input)
        var tl = scanner.lexall()
    })
    it('it should be reusable with different input', function() {
        var scanner = P2X.JScanner()
        var input = 'abc 123 \n ddds'
        scanner.str(input)
        var tl = scanner.lexall()
        var input = 'abc 123 \n ddds'
        scanner.str(input)
        var tl1 = scanner.lexall()
    })
    it('with no rules, it should return no token', function() {
        var scanner = P2X.JScanner();
        var tl = scanner.lexall();
        assert.equal(tl.list.length, 0);
    })

    it('no infinite loop because of zero length matches', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1', action: 1},
            { re: '^', action: 3},
            { re: 'abc', action: 111}
        ]
        scanner.set(scConf)
        var input = 'abc 12 \n ddds 3 dsa'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 4);
    })

    it('no infinite loop because of zero length matches', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1', action: 111},
            { re: '\\b', action: 0xb0},
            { re: '[XYZ]*', action: 0xdef},
            { re: '\\B', action: 0xb1},
            { re: 'abc', action: 0x11a}
        ]
        scanner.set(scConf)
        var input = 'abc 12 ddds 1 dsa dd'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 11);
        assert.deepEqual(tl.list.map(function(x) { return x.token }),
                         [0x11a, 0xdef, 0xb1, 111, 0xb0, 0xdef, 0xb1, 111, 0xdef, 0xb1, 0xb0 ])
        assert.deepEqual(tl.list.map(function(x) { return x.index }),
                         [0, 3, 3, 4, 5, 5, 8, 12, 13, 13, 14 ])
    })

    it('in case of length ties, the first rule wins', function() {
        var scanner = P2X.JScanner()
        scConf = [
            { re: '1',   action: 111},
            { re: '11',  action: 112},
            { re: '11*', action: 119},
            { re: '[0-9]+', action: 444},
        ]
        scanner.set(scConf)
        var input = '1111 111 11 1 1112'
        scanner.str(input)
        var tl = scanner.lexall()
        assert.equal(tl.list.length, 5);

        assert.deepEqual(tl.list.map(function(x) { return x.token }),
                         [119, 119, 112, 111, 444])

    })

    it('has UTF-8 support', function() {
        var input = fs.readFileSync('../examples/in/utf8-ident.exp')+''
        var scConf = P2X.parseJSON(fs.readFileSync('../examples/configs/scanner-c.json')+'')
        var scanner = P2X.JScanner()
        scanner.set(scConf)
        scanner.str(input)
        var tl = scanner.lexall()
        // console.dir(tl)
        // console.dir(tl.list.map(function(x) { return x.token }))
        assert.equal(tl.list.length, 9);
        assert.deepEqual(tl.list.map(function(x) { return x.token }),
                         [31,68,31,68,31,68,31,68,31])

    })

  })
})


describe('P2X.Parser', function(){
  describe('#getRMOp()', function(){
    it('it should return an expression tree', function() {

        var parser = P2X.Parser()
        var parseConf = [
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_UNARY_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]
        parser.setconfig(parseConf)

        parser.root = {token: P2X.TOKEN_ROOT,
                       left: undefined,
                       right: {
                           data: 'plus_top',
                           token: P2X.TOKEN_PLUS,
                           left: {
                               data: 'MULT_left',
                               token: P2X.TOKEN_MULT,
                               left: {
                                   token: P2X.TOKEN_INTEGER,
                                   text: '123'
                               },
                               right: {
                                   token: P2X.TOKEN_INTEGER,
                                   text: '456'
                               }
                           },
                           right: {
                               data: 'MULT_right',
                               token: P2X.TOKEN_MULT,
                               left: {
                                   data: 'MULT_right_op1',
                                   token: P2X.TOKEN_INTEGER,
                                   text: '123'
                               },
                               right: {
                                   data: 'MULT_right_op2',
                                   token: P2X.TOKEN_INTEGER,
                                   text: '456'
                               }
                           }
                       }
                      }

        parser.leastMap.insert(20, parser.root.right)
        parser.leastMap.insert(30, parser.root.right.right)
        
        var rm = parser.getRMOp()
        assert.equal(parser.tokenInfo.isOp(parser.root.right), true)
        assert.equal(parser.tokenInfo.isOp(parser.root.right.left), true)
        assert.equal(parser.tokenInfo.isOp(parser.root.right.right), true)
        assert.equal(rm.data, 'MULT_right');
    })
  })
  describe('#parse()', function(){
      var xmlres = fs.readFileSync('../examples/xml/1p2ep3.xml')+''
      // console.log(xmlres)
    var xmlres2 = fs.readFileSync('../examples/xml/l1p2r.xml')+''

    it('it should return an expression tree', function() {
        var scanner = P2X.JScanner()
        var scConf = [
            { re: '=',      action: P2X.TOKEN_EQUAL },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
        ]
        var input = '1+2=+3'
        scanner.set(scConf)
        scanner.str(input)
        var tl = scanner.lexall().mkeof()
        var parser = P2X.Parser()
        var parseConf = [
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_UNARY_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
        ]
        parser.setconfig(parseConf)
        var res = parser.parse(tl)
        var tpOptions = TPOptions();
        var tp = P2X.TreePrinter(parser.tokenInfo, tpOptions)

        res = tp.asxml(parser.root)

//        console.log(P2X.escapeBSQLines(res))
        assert.equal(res, xmlres);
    })

    it('it should return an expression tree (custom names)', function() {
        var scanner = P2X.JScanner()
        var scConf = [
            { re: '=',      action: 1101 },
            { re: '\\+',    action: 1102 },
            { re: '[0-9]+', action: 1103 },
        ]
        var input = '1+2=+3'
        scanner.set(scConf)
        scanner.str(input)
        var tl = scanner.lexall().mkeof()
        var parser = P2X.Parser()
        var parseConf = [
            { type: 1101, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500, name: 'op' },
            { type: 1102, mode: P2X.MODE_UNARY_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200, name: 'op' },
            { type: 1103, mode: P2X.MODE_ITEM, name: 'integer' },
        ]
        parser.setconfig(parseConf)
        var res = parser.parse(tl)
        var tpOptions = TPOptions();
        tpOptions.type = false
        var tp = P2X.TreePrinter(parser.tokenInfo, tpOptions)

        res = tp.asxml(parser.root)

//        console.log(P2X.escapeBSQLines(res))
        allLinesEqual(res, xmlres.replace(/ type=".*"/g, ''));
    })

    it('the same with the easy-to-use function', function() {
        // console.log('IT' + xmlres)
        var scConf = [
            { re: '=',      action: P2X.TOKEN_EQUAL },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
        ]
        var input = '1+2=+3'
        var parseConf = [
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_UNARY_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)

        // console.log(res)
        // console.log('RES' + xmlres)
        assert.equal(res, xmlres);
    })

    it('testing parentheses', function() {
        var scConf = [
            { re: '\\(',    action: P2X.TOKEN_L_PAREN },
            { re: '\\)',    action: P2X.TOKEN_R_PAREN },
            { re: '=',      action: P2X.TOKEN_EQUAL },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
        ]
        var input = '(1+2)'
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: P2X.TOKEN_L_PAREN, isParen: 1, closingList: [ { type: P2X.TOKEN_R_PAREN } ] },
                { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
            ]
        }
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)
        
        // console.log(res)
        // console.log(xmlres2)
        allLinesEqual(res, xmlres2)
    })

    it('testing ignored items', function() {
        var scConf = [
            { re: '\\(',    action: P2X.TOKEN_IGNORE },
            { re: '\\)',    action: P2X.TOKEN_IGNORE },
            { re: '=',      action: P2X.TOKEN_EQUAL },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
        ]
        var input = '1+()2'
        var parseConf = {
            ignoreIgnore: false,
            rules: [
                { type: P2X.TOKEN_L_PAREN, isParen: 1, closingList: [ { type: P2X.TOKEN_R_PAREN } ] },
                { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
            ]
        }
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        var res = P2X.p2xj(input, p2xConfig)

        var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
+' <root type=\"ROOT\">\n'
+'  <null/>\n'
+'  <op line=\"1\" col=\"1\" type=\"PLUS\">\n'
+'   <integer line=\"1\" col=\"0\" type=\"INTEGER\"><ca:text>1</ca:text></integer>\n'
+'   <ca:text>\+</ca:text>\n'
+'   <ca:ignore line=\"1\" col=\"2\" type=\"IGNORE\"><ca:text>\(</ca:text></ca:ignore>\n'
+'   <ca:ignore line=\"1\" col=\"3\" type=\"IGNORE\"><ca:text>\)</ca:text></ca:ignore>\n'
+'   <integer line=\"1\" col=\"4\" type=\"INTEGER\"><ca:text>2</ca:text></integer>\n'
+'  </op>\n'
+' </root>\n'
+'</code-xml>\n'

        assert.equal(res, check)        
    })

    it('testing JUXTA ops', function() {
        var scConf = [
            { re: ' +', action: P2X.TOKEN_IGNORE },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
        ]
        var input = '1 2 3 4'
        var parseConf = {
            ignoreIgnore: true,
            rules: [ ]
        }
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions(), debug: true}
        var result = {}
        var res = P2X.p2xj(input, p2xConfig, result)

        assert.equal(result.parseres.right.line, 1)
        assert.equal(result.parseres.right.col, 6)
    })

    it('testing postfix op', function() {
        var xmlres = fs.readFileSync('../examples/xml/ftp2.xml')+''
        var scConf = [
            { re: '\'',     action: P2X.TOKEN_QUOTE },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = 'f\'+2'
        var parseConf = [
            { type: P2X.TOKEN_QUOTE, mode: P2X.MODE_POSTFIX, prec: 3000 },
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)
        assert.equal(res, xmlres)
    })

    it('testing named postfix op', function() {
        var xmlres = fs.readFileSync('../examples/xml/ftp3.xml')+''
        var scConf = [
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = 'f T + 2'
        var parseConf = [
            { type: P2X.TOKEN_IDENTIFIER, repr: 'T', mode: P2X.MODE_POSTFIX, prec: 3000 },
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]
        var tpopts = TPOptions()
        tpopts.line = false
        tpopts.col = false
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: tpopts}
        res = P2X.p2xj(input, p2xConfig)

        // console.log(res)
        // console.log(xmlres)
        allLinesEqual(res, xmlres)
    })

    it('testing postfix op (missing prec)', function() {
        var xmlres = fs.readFileSync('../examples/xml/ftp2.xml')+''
        var scConf = [
            { re: '\'',     action: P2X.TOKEN_QUOTE },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = 'f\'+2'
        var parseConf = [
            { type: P2X.TOKEN_QUOTE, mode: P2X.MODE_POSTFIX },
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)

        // console.log(res)
        assert.equal(res, xmlres)
    })

    it('testing right associative binary', function() {
        var xmlres = fs.readFileSync('../examples/xml/ftp2.xml')+''
        var scConf = [
            { re: '=',     action: P2X.TOKEN_EQUAL },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = 'f=g=2'
        var parseConf = { rules: [
            { type: P2X.TOKEN_QUOTE, mode: P2X.MODE_POSTFIX },
            { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
            { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]}
        var p2xConfig = {debug: 1, scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        var info = {}
        res = P2X.p2xj(input, p2xConfig, info)
        var root = info.parseres
        assert.equal(root.token, P2X.TOKEN_ROOT)
        assert.equal(root.right.token, P2X.TOKEN_EQUAL)
        assert.equal(root.right.left.text, 'f')
        assert.equal(root.right.right.token, P2X.TOKEN_EQUAL)
        assert.equal(root.right.right.left.text, 'g')
        assert.equal(root.right.right.right.text, '2')
    })

      var xmlres_un = '<code-xml xmlns=\'http://ai-and-it.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it.de/xml/code-xml/attributes/\' ca:version=\'1.0\'>\n'
+' <root type="ROOT">\n'
+'  <null/>\n'
+'  <op line="1" col="3" type="MULT">\n'
+'   <op line="1" col="0" type="MINUS">\n'
+'    <null/>\n'
+'    <ca:text>-</ca:text>\n'
+'    <integer line="1" col="1" type="INTEGER"><ca:text>1</ca:text></integer>\n'
+'   </op>\n'
+'   <ca:text>*</ca:text>\n'
+'   <integer line="1" col="5" type="INTEGER"><ca:text>2</ca:text></integer>\n'
+'  </op>\n'
+' </root>\n'
+'</code-xml>\n'
    it('testing unary op', function() {
        var scConf = [
            { re: '-',     action: P2X.TOKEN_MINUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = '-1 * 2'
        var parseConf = [
            { type: P2X.TOKEN_MINUS, mode: P2X.MODE_UNARY, prec: 3000 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)
        
//        console.log(P2X.escapeBSQLines(res, '\''))
        assert.equal(res, xmlres_un)
    })

    it('testing unary_binary op', function() {
        var scConf = [
            { re: '-',     action: P2X.TOKEN_MINUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = '-1 * 2'
        var parseConf = [
            { type: P2X.TOKEN_MINUS, mode: P2X.MODE_UNARY_BINARY, prec: 1000, precU: 3000 },
            { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)
        
        // console.log(P2X.escapeBSQLines(res, '\''))
        assert.equal(res, xmlres_un)
    })

      it('testing binary parenthesis', function() {
        var xmlres = fs.readFileSync('../examples/xml/fl1rg.xml')+''
        var scConf = [
            { re: '\\(',    action: P2X.TOKEN_L_PAREN },
            { re: '\\)',    action: P2X.TOKEN_R_PAREN },
            { re: '=',      action: P2X.TOKEN_EQUAL },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = 'f(1)g'
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: P2X.TOKEN_L_PAREN, mode: P2X.MODE_BINARY, prec: 1050,
                  isParen: 1, closingList: [ { type: P2X.TOKEN_R_PAREN } ] },
                { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
            ]
        }
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)

//        console.log(res)
        allLinesEqual(res, xmlres)
    })

    it('testing postfix parenthesis', function() {
        var xmlres = fs.readFileSync('../examples/xml/f1.xml')+''
        var scConf = [
            { re: '\\(',    action: P2X.TOKEN_L_PAREN },
            { re: '\\)',    action: P2X.TOKEN_R_PAREN },
            { re: '=',      action: P2X.TOKEN_EQUAL },
            { re: '\\+',    action: P2X.TOKEN_PLUS },
            { re: '\\*',    action: P2X.TOKEN_MULT },
            { re: '[0-9]+', action: P2X.TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: P2X.TOKEN_IDENTIFIER },
        ]
        var input = 'f(1)'
        var parseConf = {
            ignoreIgnore: true,
            rules: [
                { type: P2X.TOKEN_L_PAREN, mode: P2X.MODE_POSTFIX, prec: 3000, isParen: 1, closingList: [ { type: P2X.TOKEN_R_PAREN } ] },
                { type: P2X.TOKEN_EQUAL, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_RIGHT, prec: 500 },
                { type: P2X.TOKEN_PLUS, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1000, precU: 2200 },
                { type: P2X.TOKEN_MULT, mode: P2X.MODE_BINARY, assoc: P2X.ASSOC_LEFT, prec: 1100 },
            ]
        }
        var p2xConfig = {scanner: scConf, parser: parseConf, treewriter: TPOptions()}
        res = P2X.p2xj(input, p2xConfig)

//        console.log(res)
        allLinesEqual(res, xmlres)
    })


  })
})

describe('P2X.TreePrinterOptions', function(){
    describe('#construct()', function(){
        it('Should honour existing fields in the first argument', function(){
            tpOptions1 = { line: 0, col: 0, type: 0 };
            tpOptions2 = P2X.TreePrinterOptions(tpOptions1);
            Object.keys(tpOptions1).map(function(k){
                assert(k in tpOptions1)
                assert(k in tpOptions2)
                assert.equal(tpOptions1[k], tpOptions2[k])
            })
        })
    })
})

describe('P2X.TreePrinter', function(){
    describe('#asxml()', function(){
        var tree = P2X.Token(P2X.TOKEN_ROOT)
        tree.left = P2X.Token(P2X.TOKEN_PLUS, '+')
        tree.left.left = P2X.Token(P2X.TOKEN_INTEGER, '1')
        tree.left.right = P2X.Token(P2X.TOKEN_INTEGER, '2')
        
        var tree2 = P2X.Token(P2X.TOKEN_ROOT)
        tree2.left = P2X.Token(1100, '+')
        tree2.left.left = P2X.Token(1101, '1')
        tree2.left.right = P2X.Token(1101, '2')

        var result = {}
        result.scanner = P2X.JScanner()
        result.parser = P2X.Parser()

        var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\''
            + ' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
            +' <root type=\"ROOT\">\n'
            +'  <op type=\"PLUS\">\n'
            +'   <integer type=\"INTEGER\"><ca:text>1</ca:text></integer>\n'
            +'   <ca:text>\+</ca:text>\n'
            +'   <integer type=\"INTEGER\"><ca:text>2</ca:text></integer>\n'
            +'  </op>\n'
            +' </root>\n'
            +'</code-xml>\n'

        it('It prints an XML tree of the object', function(){
            var tp = P2X.TreePrinter()
            var res = tp.asxml(undefined)
            // console.log(P2X.escapeBSQLines(res))
            var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
+'</code-xml>\n'
            assert.equal(res, check)
        })
        it('It prints an XML tree of the object', function(){
            var tp = P2X.TreePrinter()
            var res = tp.asxml(1)
            // console.log(P2X.escapeBSQLines(res))
            var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
+' <op type=\"number\"><ca:text>1</ca:text></op>\n'
+'</code-xml>\n'
            assert.equal(res, check)
        })
        it('It prints an XML tree of the object', function(){
            var tp = P2X.TreePrinter()
            var res = tp.asxml({token: P2X.TOKEN_ROOT})
            // console.log(P2X.escapeBSQLines(res))
            var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
+' <root type=\"ROOT\"></root>\n'
+'</code-xml>\n'
            assert.equal(res, check)
        })
        it('It prints an XML tree of the object', function(){
            var tp = P2X.TreePrinter()
            var res = tp.asxml({token: P2X.TOKEN_ROOT, right: { token: P2X.TOKEN_INTEGER, text: '2'}})
            // console.log(P2X.escapeBSQLines(res))
            var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
+' <root type=\"ROOT\">\n'
+'  <null/>\n'
+'  <integer type=\"INTEGER\"><ca:text>2</ca:text></integer>\n'
+' </root>\n'
+'</code-xml>\n'
            assert.equal(res, check)
        })
        it('It prints an XML tree of the object', function(){
            var topts = TPOptions()
            var tp = P2X.TreePrinter(undefined, topts)
            var res = tp.asxml(tree)
            allLinesEqual(res, check)
        })
        it('It prints an XML tree of the object', function(){
            var tp = P2X.TreePrinter()
            var res = tp.asxml(tree)
            allLinesEqual(res, check)
        })
        it('It prints an XML tree of the object', function(){
            var tp = P2X.TreePrinter()
            var res = tp.asxml(tree2)
            var check = '<code-xml xmlns=\'http://ai-and-it\.de/xml/code-xml/\' xmlns:ca=\'http://ai-and-it\.de/xml/code-xml/attributes/\' ca:version=\'1\.0\'>\n'
+' <root type=\"ROOT\">\n'
+'  <op type=\"1100\">\n'
+'   <op type=\"1101\"><ca:text>1</ca:text></op>\n'
+'   <ca:text>\+</ca:text>\n'
+'   <op type=\"1101\"><ca:text>2</ca:text></op>\n'
+'  </op>\n'
+' </root>\n'
+'</code-xml>\n'
            assert.equal(res, check)
        })
        it('With option caSteps, the element can be included', function(){
            var opts = TPOptions()
            opts.caSteps = true
            var tp = P2X.TreePrinter(undefined, opts)
            var res = tp.asxml(1)
            assert(res.indexOf('<ca:steps/>') > -1)
        })
        it('With option caSteps, the element can be included', function(){
            var opts = TPOptions()
            opts.caSteps = false
            var tp = P2X.TreePrinter(undefined, opts)
            var res = tp.asxml(1)
            assert(res.indexOf('<ca:steps/>') == -1)
        })
        it('With option scanConf, the element can be included', function(){
            var opts = TPOptions()
            opts.scanConf = true
            var tp = P2X.TreePrinter(undefined, opts)
            var res = tp.asxml(tree, ' ', result)
            assert(res.indexOf('<ca:scanner>') > -1)
            assert(res.indexOf('</ca:scanner>') > -1)
        })
        it('With option scanConf, the element can be included', function(){
            var opts = TPOptions()
            opts.scanConf = false
            var tp = P2X.TreePrinter(undefined, opts)
            var res = tp.asxml(tree, ' ', result)
            assert(res.indexOf('<ca:scanner>') == -1)
            assert(res.indexOf('</ca:scanner>') == -1)
        })
        it('With option parseConf, the element can be included', function(){
            var opts = TPOptions()
            opts.parseConf = true
            var tp = P2X.TreePrinterPlus(undefined, opts)
            var res = tp.asxml(tree, ' ', result)
            assert(res.indexOf('<ca:parser>') > -1)
            assert(res.indexOf('</ca:parser>') > -1)
        })
        it('With option parseConf, the element can be included', function(){
            var opts = TPOptions()
            opts.parseConf = false
            var tp = P2X.TreePrinter(undefined, opts)
            var res = tp.asxml(tree)
            assert(res.indexOf('<ca:parser>') == -1)
            assert(res.indexOf('</ca:parser>') == -1)
        })
        it('With indent, indentation can be turned off', function(){
            var topts = TPOptions()
            var tp = P2X.TreePrinter(undefined, topts)
            topts.indent = false
            var res = tp.asxml(tree)
            allLinesEqual(res, check.replace(/ (?!.*>)/g, ''))
        })
    })
})

describe('P2X.TextUtils', function(){
    describe('#escapeBSQLines()', function(){
        it('should be identity with eval for strings', function(){
            var str = 'ab\ncde'
            var qlines = '\'ab\\n\'\n+\'cde\''
            assert.equal(P2X.escapeBSQLines(str), qlines);
        })
        it('should be identity with eval for strings', function(){
            var str = 'ab\ncd\ne'
            var qlines = '\'ab\\n\'\n+\'cd\\n\'\n+\'e\''
            assert.equal(P2X.escapeBSQLines(str), qlines);
        })
        it('should be identity with eval for strings', function(){
            var str = 'ab\n cd\n e'
            var qlines = '\'ab\\n\'\n+\' cd\\n\'\n+\' e\''
            assert.equal(P2X.escapeBSQLines(str), qlines);
        })
        it('should be identity with eval for strings', function(){
            var str = 'abcde\n'
            var qlines = '\'abcde\\n\''
            assert.equal(P2X.escapeBSQLines(str), qlines);
        })
        it('should be identity with eval for strings', function(){
            var str = 'abc\n\nde'
            var qlines = '\'abc\\n\'\n+\'\\nde\''
            assert.equal(P2X.escapeBSQLines(str), qlines);
        })
        it('should be identity with eval for strings', function(){
            var str = 'abc\n\n\n\nde'
            var qlines = '\'abc\\n\'\n+\'\\n\\n\\nde\''
            assert.equal(P2X.escapeBSQLines(str), qlines);
        })
    })
    describe('#escapeBS()', function(){
        it('should be identity with eval for strings', function(){
            var str = 'abcde'
            assert.equal(str, eval('\'' + P2X.escapeBS(str) + '\''));
        })
        it('should be identity with eval for strings', function(){
            var str = '\\abcde'
            assert.equal(str, eval('\'' + P2X.escapeBS(str) + '\''));
        })
        it('should be identity with eval for strings', function(){
            var str = '\\abcd\"e'
            // console.log('str::>' + str + '<\n')
            // console.log('str::>' + P2X.escapeBS(str) + '<\n')
            assert.equal(str, eval('\'' + P2X.escapeBS(str) + '\''));
        })
        it('should be identity with eval for strings', function(){
            var str = '\\abcd\"\'e'
            assert.equal(str, eval('\'' + P2X.escapeBS(str) + '\''));
        })
        it('should be identity with eval for strings', function(){
            var str = '\\a\n\r\\\tbcd\"\'e'
            assert.equal(str, eval('\'' + P2X.escapeBS(str) + '\''));
        })
    })
})

describe('P2X.parseOptions', function(){
    var POpts = require('../parse-opts.js')
    var optDefs = [
        { short: 'p', long: 'prec-list' },
        { short: 's', long: 'scan-only' },
        { short: 'S', long: 'scanner-config' },
        { short: 'c', long: 'config' },
        { short: 'o', long: 'outfile' },
        { short: 'C', long: 'include-config', flag: 1 },
    ]

    var argv = ['', '', '-c', 'test.conf', 'text.txt', 'text2.txt']

    it('should return an object with parsed options', function(){
        options = POpts.parseOptions(argv, optDefs)
        assert.deepEqual(options['include-config'], undefined)
        assert.deepEqual(options['config'], ['test.conf'])
        assert.equal(options['arguments'][0], 'text.txt')
        assert.equal(options['arguments'][1], 'text2.txt')
    })

    it('flags do not consume an argument', function(){
        var argv = ['', '', '-C', '-c', 'test.conf', 'text.txt', 'text2.txt']
        options = POpts.parseOptions(argv, optDefs)
        assert.deepEqual(options['include-config'], [1])
        assert.deepEqual(options['config'], ['test.conf'])
        assert.equal(options['arguments'][0], 'text.txt')
        assert.equal(options['arguments'][1], 'text2.txt')
    })
})

describe('P2X.CLI', function(){
  describe('#p2xjs()', function(){
    var xmlres = fs.readFileSync('../examples/xml/1p2ep3.xml')+''
    var xmlres2 = fs.readFileSync('../examples/xml/l1p2r.xml')+''

    var pres1, pres2, pres3
    var mode = '';
    var p2x_options = '';
      
    function runP2XJS(scanConfigFile, configFile, inputFile, done) {
        var cmd = './p2xjs' + ' ' + p2x_options + (mode ? ' ' + mode : '')
            + (scanConfigFile ? ' -S ' + scanConfigFile : '')
            + (configFile ? ' -p ' + configFile : '')
            + (inputFile ? ' ' + inputFile : '')
        console.log('run ' + cmd)
        // system(cmd)
        var child = child_process.exec(cmd, { stdio: 'inherit' },
                                       function(errc, stdout, stderr)
                                       {
                                           if (errc) {
                                               console.error('P2X exited with error:\nerrc: ' + errc + '\nstderr: ' + stderr)
                                               assert(false);
                                           } else {
                                               // console.log('P2X exited2 errc::' + errc + ':: stdout::' + stdout + '::')
                                           }
                                           done(stdout)
                                       })
    }
      
    function runP2XJSNew(p2xConfigFile, inputFile, done) {
        var cmd = './p2xjs' + ' ' + p2x_options + ' -c ' + p2xConfigFile + ' ' + inputFile
        console.log('run ' + cmd)
        // system(cmd)
        var child = child_process.exec(cmd, { stdio: 'inherit' },
                                       function(errc, stdout, stderr)
                                       {
                                           if (errc) {
                                               console.error('P2X exited with error:\n' + errc + stderr)
                                               assert(false);
                                           } else {
                                               // console.log('P2X exited2 errc::' + errc + ':: stdout::' + stdout + '::')
                                           }
                                           done(stdout)
                                       })
    }

    it('p2xjs client command', function(done) {
        var scanConfigFile = '../examples/configs/scanner-c.xml'
        var configFile = '../examples/configs/default'
        var inputFile = '../examples/in/postfix1.exp'
        runP2XJS(scanConfigFile, configFile, inputFile, function(res) { pres1 = res; done() })
    })

    it('p2xjs client command, with JSON config files', function(done) {
        var scanConfigFile = '../examples/configs/scanner-c.json'
        var configFile = '../examples/configs/default'
        var inputFile = '../examples/in/postfix1.exp'
        runP2XJS(scanConfigFile, configFile, inputFile, function(res) {
            assert.equal(pres1, res)
            done()
        })
    })

    xit('a shortcut may be used for the scanner config', function(done) {
        var scanConfigFile = 'c'
        var configFile = '../examples/configs/default'
        var inputFile = '../examples/in/postfix1.exp'
        runP2XJS(scanConfigFile, configFile, inputFile, function(res) {
            assert.equal(pres1, res)
            done()
        })
    })

    it('scanner config file can be converted to JSON separately', function(done) {
        var scanConfigFile = '../examples/configs/scanner-c.json'
        var scanConfigFileXML = '../examples/configs/scanner-c.xml'
        mode = 'scanconf-xml2json'
        runP2XJS(scanConfigFileXML, '', '', function(res) {
            fs.readFile(scanConfigFile, function(err, data) {
                if (err) throw err
                assert.equal(res, data + '')
                done()
            })
        })
    })

    it('scanner config file can be converted to JSON separately, used as config file', function(done) {
        var scanConfigFile = '../examples/configs/scanner-c.xml'
        var scanConfigFileJSON = '/tmp/scanner-c.json'
        var configFile = '../examples/configs/default'
        var inputFile = '../examples/in/postfix1.exp'
        mode = 'scanconf-xml2json'
        runP2XJS(scanConfigFile, undefined, undefined, function(res) {
            fs.writeFile(scanConfigFileJSON, res, function(err) {
                if (err) throw err;
                mode = ''
                runP2XJS(scanConfigFileJSON, configFile, inputFile, function(res) {
                    assert.equal(pres1, res)
                    done()
                })
            })
        })
    })

    it('another test with JSON config files (prepare unified)', function(done) {
        var scanConfigFile = '../examples/configs/unified-scanner.json'
        var configFile = '../examples/configs/unified-parser.json'
        // var p2xConfigFile = '../examples/configs/unified.json'
        var inputFile = '../examples/in/postfix1.exp'
        runP2XJS(scanConfigFile, configFile, inputFile, function(res) {
            pres3 = res
            done()
        })
    })

    it('config can be given as a unified P2X file', function(done) {
        // var scanConfigFile = '../examples/configs/unified-scanner.json'
        // var configFile = '../examples/configs/unified-parser.json'
        var p2xConfigFile = '../examples/configs/unified.json'
        var inputFile = '../examples/in/postfix1.exp'
        runP2XJSNew(p2xConfigFile, inputFile, function(res) {
            allLinesEqual(pres3, res)
            done()
        })
    })

    it('flag -C can dump the config status to the XML', function(done) {
        var p2xConfigFile = '../examples/configs/unified.json'
        var inputFile = '../examples/in/postfix1.exp'
        p2x_options = '-C'
        runP2XJSNew(p2xConfigFile, inputFile, function(res) {
            assert(res.indexOf('<ca:scanner') > -1)
            assert(res.indexOf('<ca:parser') > -1
                   || res.indexOf('<!-- printing the parser config as is not supported') > -1)
            assert(res.indexOf('<ca:tree-writer') > -1)
            assert(res.indexOf('<id type="1004"><ca:text>s</ca:text></id>') > -1)
            done()
        })
    })

  })
})


describe('P2X.HashMap', function(){
  P2X.HashMap = require('../hashmap.js')
  describe('#construct()', function(){
    it('empty array should have length 0', function(){
        assert.equal(P2X.HashMap.HashMap().length(), 0);
    })
  })
  describe('#insert()', function(){
      it('array maps are created', function(){
          var x = P2X.HashMap.HashMap()
          var nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
          nums.map(function(k) {
              x.insert(k*10, k)
          })
          nums.map(function(k) {
              assert.equal(x.get(k*10), k)
          })
      })
  })
  describe('#insert().error', function(){
      it('checks index is not too large', function(){
          var x = P2X.HashMap.HashMap()
          var ok = false
          try {
              x.insert(1000, 23)
          } catch (e) {
              ok = true
          }
          assert.equal(ok, true)
      })
      it('checks index has type number', function(){
          var x = P2X.HashMap.HashMap()
          var ok = false
          try {
              x.insert('10', 23)
          } catch (e) {
              ok = true
          }
          assert.equal(ok, true)
      })
      it('checks index is Integer', function(){
          var x = P2X.HashMap.HashMap()
          var ok = false
          try {
              x.insert(10.23, 23)
          } catch (e) {
              ok = true
          }
          assert.equal(ok, true)
      })
      it('checks index is not undefined', function(){
          var x = P2X.HashMap.HashMap()
          var ok = false
          try {
              x.insert(zzz, 23)
          } catch (e) {
              ok = true
          }
          assert.equal(ok, true)
      })
  })
  describe('#insert().order', function(){
      it('Indices are ordered', function(){
          var x = P2X.HashMap.HashMap(1021)
          x.insert(1000, 23)
          x.insert(1020, 23)
          x.insert(1010, 23)
          assert.equal(x.first(x.begin()), 1000)
          assert.equal(x.first(x.begin()+1), 1010)
          assert.equal(x.first(x.begin()+2), 1020)
      })
  })
  describe('#construct().paramMaxIndex', function(){
      it('max index can be increased', function(){
          var x = P2X.HashMap.HashMap(1001)
          var ok = true
          x.insert(1000, 23)
      })
  })
  describe('#iterators()', function(){
      it('map iterators', function(){
          var x = P2X.HashMap.HashMap()
          var nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
          nums.map(function(k) {
              x.insert(k*10, k)
          })
          assert.equal(x.begin(), 0)
          assert.equal(x.end(), 10)
          assert.equal(x.first(x.begin()), 10)
          assert.equal(x.second(x.begin()), 1)
          assert.equal(x.first(x.begin()+1), 20)
          assert.equal(x.second(x.begin()+1), 2)
    })
  })
  describe('#find()', function(){
      it('array maps are created', function(){
          var x = P2X.HashMap.HashMap()
          var nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
          nums.map(function(k) {
              x.insert(k*10, k)
          })
          assert.equal(x.find(30), 3)
          assert.equal(x.find(20), 2)
          assert.equal(x.find(32), x.end())
    })
  })
  describe('#keys()', function(){
      var x = P2X.HashMap.HashMap(1e8)
      var nums = [1000, 500, 100, 10]
      nums.map(function(k) {
          x.insert(k*10, k)
      })
      nums = nums.sort(x.cmp)
      it('returns ordered keys', function(){
          assert.equal(String(x.keys()), ''+nums.map(function(k){return String(k*10)}))
      })
  })
  describe('#lower_bound()', function(){
      var x = P2X.HashMap.HashMap()
      var nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
      nums.map(function(k) {
          x.insert(k*10, k)
      })
      it('array maps are created', function(){
          assert.equal(x.lower_bound(30), 3)
      })
      it('array maps are created', function(){
          assert.equal(x.lower_bound(29), 3)
      })
      it('array maps are created', function(){
          assert.equal(x.lower_bound(21), 3)
      })
      it('array maps are created', function(){
          assert.equal(x.lower_bound(20), 2)
      })
  })
  describe('#erase()', function(){
      var x = P2X.HashMap.HashMap()
      var nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
      nums.map(function(k) {
          x.insert(k*10, k)
      })
      var xg = x
      it('array maps erase func', function(){
          var x = xg.clone()
          var t = x.erase(x.lower_bound(60), x.end())
          for (var i = 0;  i< 6; ++i) {
              assert.equal(x.lower_bound(nums[i]*10) < x.end(), true)
          }
          for (var i = 0;  i< 5; ++i) {
              assert.equal(x.lower_bound(nums[i+6]*10), x.end())
          }
      })
      it('array maps erase func', function(){
          var x = xg.clone()
          var t = x.erase(x.lower_bound(59), x.end())
          for (var i = 0;  i< 6; ++i) {
              assert.equal(x.lower_bound(nums[i]*10) < x.end(), true)
          }
          for (var i = 0;  i< 5; ++i) {
              assert.equal(x.lower_bound(nums[i+6]*10), x.end())
          }
      })
      it('array maps erase func', function(){
          var x = xg.clone()
          var t = x.erase(x.lower_bound(61), x.end())
          for (var i = 0;  i< 7; ++i) {
              assert.equal(x.lower_bound(nums[i]*10) < x.end(), true)
          }
          for (var i = 0;  i< 4; ++i) {
              assert.equal(x.lower_bound(nums[i+7]*10), x.end())
          }
      })
  })
})

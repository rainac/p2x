
var assert = require("assert")
var fs = require("fs")

describe('Array', function(){
  describe('#indexOf()', function(){
    it('should return -1 when the value is not present', function(){
      assert.equal(-1, [1,2,3].indexOf(5));
      assert.equal(-1, [1,2,3].indexOf(0));
    })
  })
})

var P2X = require("../scanner.js")

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
          var xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          var scConf = P2X.ScannerConfig().loadXML(xmlRes)
          console.dir(scConf)
          assert.equal(xmlRes, scConf.asxml(''));
      })
      it('on invalid input, no rule entry should be created', function(){
          var xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><rea>abc</rea><actions>1023</actions></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          var xmlResEmpty = '<ca:scanner-config>\n'
              + '</ca:scanner-config>\n'
          var scConf = P2X.ScannerConfig().loadXML(xmlRes)
          assert.equal(scConf.length, 0);
          assert.equal(xmlResEmpty, scConf.asxml(''));
      })
      it('should return XML rule list', function(){
          var xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>abc/</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc/</ca:re><ca:action>TOKEN_IDENTIFIER</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc/</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          scConf = P2X.ScannerConfig().loadXML(xmlRes)
          console.log(xmlRes)
          console.dir(scConf)
          console.log(scConf.asxml(''))
          assert.equal(xmlRes, scConf.asxml(''));
      })
      function setAndGetFromScanner(scConf) {
          console.dir(scConf)
          var myScanner = P2X.JScanner();
          myScanner.set(scConf)
          console.dir(myScanner.actions)
          console.dir(myScanner.get())
          return myScanner.get()
      }
      function setAndGetFromScannerXML(inXml) {
          var scConf = P2X.ScannerConfig().loadXML(inXml)
          return setAndGetFromScanner(scConf).asxml('')
      }
      it('should return XML rule list', function(){
          var inXml = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>11</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          var xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_KEYW_FUNCTION</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          assert.equal(xmlRes, setAndGetFromScannerXML(inXml));
      })
      it('should return XML rule list', function(){
          var inXml = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>\\(</ca:re><ca:action>TOKEN_MULT</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\'</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\\\\</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>\\)</ca:re><ca:action>TOKEN_MINUS</ca:action></ca:lexem>\n'
              + '<ca:lexem><ca:re>&amp;</ca:re><ca:action>TOKEN_PLUS</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
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
  })
  describe('#asxml()', function(){
    it('should return empty XML', function(){
        assert.equal('<ca:scanner-config>\n</ca:scanner-config>\n',
                     P2X.ScannerConfig([]).asxml(''));
    })
      it('should return XML rule list', function(){
          var xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>1023</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          var scConf = [
              {re: /abc/, action: 1023},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>abc</ca:re><ca:action>TOKEN_DIV_EQUAL</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          scConf = [
              {re: /abc/, action: 100},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>a+b*c?</ca:re><ca:action>TOKEN_DOUBLE_LESS_EQUAL</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          action = 112
          scConf = [
              {re: /a+b*c?/, action: TOKEN_DOUBLE_LESS_EQUAL},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>ab\\\\c\\b</ca:re><ca:action>function () { return action*2 }</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
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
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><ca:re>a&lt;&gt;+b&lt;&amp;*c?</ca:re><ca:action>TOKEN_DOUBLE_LESS_EQUAL</ca:action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          action = 102
          scConf = [
              {re: /a<>+b<&*c?/, action: TOKEN_DOUBLE_LESS_EQUAL},
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
        it('should return XML rule list', function(){
            xmlRes = '<ca:scanner-config>\n'
                + '<ca:lexem><ca:re>/abc/</ca:re><ca:action>function () { return action*2 }</ca:action></ca:lexem>\n'
                + '</ca:scanner-config>\n'
            var tt = P2X.TokenInfo()
            var p1 = P2X.TokenProto(TOKEN_DIV, '/', MODE_BINARY, ASSOC_LEFT, 100, 0, false)
            tt.insert(p1)

            var tprw = P2X.TokenProtoRW()
            var pcrw = P2X.ParserConfigRW()
            // console.log(p1)
            // console.log(tprw.asxml(p1, ''))
            confA = tt.getconfig()
            confB = pcrw.loadXML(pcrw.asxml(confA, ''))
            // console.log(TOKEN_ROOT)
            // console.log(TOKEN_DIV)
            // console.log(pcrw.asxml(confA, ''))
            // console.log(pcrw.asxml(confA, ''))
            assert.equal(pcrw.asxml(confA, ''), pcrw.asxml(confB, ''));
            assert.equal(confA.length, confB.length);
            for (k = 0; k < confA.length; ++k) {
                // console.log(confA[k])
                // console.log(confB[k])
                for (j in confA[k]) {
                    if (confA[k][j] || confB[k][j]) {
                        // console.log('==' + confA[k][j] + ',' + confB[k][j])
                        // console.log('==' + (confA[k][j] == confB[k][j]))
                        assert.equal(confA[k][j], confB[k][j]);
                    }
                }
                // console.log('==' + k + ': ' + (confA[k] == confB[k]))
            }
            // console.log('==: ' + (confA == confB))
        })
        it('should return XML rule list', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(TOKEN_DIV, '/', MODE_BINARY, ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(TOKEN_MULT, '*', MODE_BINARY, ASSOC_LEFT, 100, 0, false))
            tt.insert(P2X.TokenProto(TOKEN_PLUS, '+', MODE_BINARY, ASSOC_LEFT, 90, 0, false))
            tt.insert(P2X.TokenProto(TOKEN_MINUS, '-', MODE_BINARY, ASSOC_LEFT, 90, 0, false))
            tt.insert(P2X.TokenProto(TOKEN_EQUAL, '=', MODE_BINARY, ASSOC_RIGHT, 50, 0, false))
            tt.insert(P2X.TokenProto(TOKEN_MINUS, '=', MODE_UNARY_BINARY, ASSOC_RIGHT, 50, 110, false))
            tt.insert(P2X.TokenProto(TOKEN_L_PAREN, '=', MODE_POSTFIX, ASSOC_NONE, 50, 0, true, [P2X.TokenProto(TOKEN_R_PAREN)]))
            tt.insert(P2X.TokenProto(TOKEN_SPACE, '=', MODE_IGNORE))
            tt.insert(P2X.TokenProto(TOKEN_NEWLINE, '=', MODE_IGNORE))
            tt.insert(P2X.TokenProto(TOKEN_CRETURN, '=', MODE_IGNORE))
            tt.insert(P2X.TokenProto(TOKEN_DIV, '/', MODE_BINARY, ASSOC_LEFT, 100, 0, false))
            var pcrw = P2X.ParserConfigRW()
            confA = tt.getconfig()
            confB = pcrw.loadXML(pcrw.asxml(confA, ''))
            // console.log(pcrw.asxml(tt.getconfig(), ''))
            // console.log(pcrw.asxml(confA, ''))
            // console.log(pcrw.asxml(confB, ''))
            assert.equal(pcrw.asxml(confA, ''), pcrw.asxml(confB, ''));
        })
        it('should return XML rule list', function(){
            var pcrw = P2X.ParserConfigRW()
            var confA = pcrw.loadXML(fs.readFileSync('test1.xml'))
            var confB = pcrw.loadXML(pcrw.asxml(confA, ''))
//            console.log(pcrw.asxml(confA, ''))
            assert.equal(pcrw.asxml(confA, ''), pcrw.asxml(confB, ''));
        })
        it('rules for ROOT and JUXTA are always present', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                P2X.TokenProto(TOKEN_DIV, '/', MODE_BINARY, ASSOC_LEFT, 100, 0, false),
                P2X.TokenProto(TOKEN_MULT, '*', MODE_BINARY, ASSOC_LEFT, 100, 0, false)
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
        it('undefined prec defaults to 1', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(TOKEN_TILDE, '~', MODE_UNARY, ASSOC_NONE, undefined, undefined, false))
            tt.insert(P2X.TokenProto(TOKEN_PLUS, '/', MODE_UNARY_BINARY, ASSOC_LEFT, undefined, undefined, false))
            tt.insert(P2X.TokenProto(TOKEN_MULT, '*', MODE_BINARY, ASSOC_LEFT, undefined, undefined, false))
            assert.equal(tt.prec(TOKEN_TILDE), 2)
        })
        it('for plain token, opcode == type', function(){
            var tt = P2X.TokenInfo()
            assert.equal(tt.getOpCode(TOKEN_TILDE), TOKEN_TILDE)
            tt.insert(P2X.TokenProto(TOKEN_TILDE, '~', MODE_UNARY, ASSOC_NONE, undefined, undefined, false))
            assert.equal(tt.getOpCode(TOKEN_TILDE), TOKEN_TILDE)
        })
        it('ROOT prec is 1', function(){
            var tt = P2X.TokenInfo()
            tt.insert(P2X.TokenProto(TOKEN_TILDE, '~', MODE_UNARY, ASSOC_NONE, undefined, undefined, false))
            // console.log(tt.tokenClasses)
            // console.log(tt.get(TOKEN_ROOT))
            assert.equal(tt.prec(TOKEN_ROOT), 1)
        })
        it('ROOT prec is 1', function(){
            try {
                var tt = P2X.TokenInfo()
                tt.insert(P2X.TokenProto(TOKEN_ROOT, '/', MODE_UNARY, ASSOC_NONE, 100, undefined, false))
                tt.insert(P2X.TokenProto(TOKEN_TILDE, '~', MODE_UNARY, ASSOC_NONE, undefined, undefined, false))
                assert.equal(0, 1)
            } catch (ME) {
            }
        })
        it('it should be possible to change the prec, assoc of JUXTA', function() {
            var tt = P2X.TokenInfo()
            var confSet = [
                P2X.TokenProto(TOKEN_JUXTA, '__JUXTA__', MODE_BINARY, ASSOC_RIGHT, 111, 0, false),
                P2X.TokenProto(TOKEN_DIV, '/', MODE_BINARY, ASSOC_LEFT, 100, 0, false),
                P2X.TokenProto(TOKEN_MULT, '*', MODE_BINARY, ASSOC_LEFT, 100, 0, false)
            ]
            var confA = tt.getconfig()
            tt.setconfig(confSet);
            var confB = tt.getconfig()
            var tokenInListA = confA.map(function(x) { return x.token })
            var tokenInListB = confB.map(function(x) { return x.token })
            for (var k in tokenInListA) {
                assert(tokenInListB.indexOf(tokenInListA[k]) > -1)
            }
            assert.equal(tt.mode(TOKEN_JUXTA), MODE_BINARY)
            assert.equal(tt.prec(TOKEN_JUXTA), 111)
            assert.equal(tt.assoc(TOKEN_JUXTA), ASSOC_RIGHT)
        })
    })
})


describe('P2X.TokenList', function(){
  describe('#construct()', function(){
    // it('should contain values in list given to it', function(){
    //     assert.equal([1, 2, 3], Array.apply({},P2X.ScannerConfig([1,2,3])));
    // })
  })
  describe('#loadXML()', function(){
      it('should return XML rule list', function(){
          xmlRes = "<scan-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'>\n"
              + '<token line="1" col="0" index="0" type="IDENTIFIER"><ca:text>a</ca:text></token>\n'
              + '<token line="1" col="1" index="1" type="SPACE"><ca:text> </ca:text></token>\n'
              + '<token line="1" col="2" index="2" type="PLUS"><ca:text>+</ca:text></token>\n'
              + '</scan-xml>\n'
          tl = P2X.TokenList.prototype.loadXML(xmlRes)
          assert.equal(xmlRes, tl.asxml());
      })
      it('should return XML rule list', function(){
          xmlRes = "<scan-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'>\n"
              + '<token line="1" col="0" index="0" type="IDENTIFIER"><ca:text>&lt;&gt;&amp;</ca:text></token>\n'
              + '<token line="1" col="1" index="1" type="NEWLINE"><ca:br/></token>\n'
              + '<token line="1" col="2" index="2" type="CRETURN"><ca:cr/></token>\n'
              + '</scan-xml>\n'
          tl = P2X.TokenList.prototype.loadXML(xmlRes)
          assert.equal(xmlRes, tl.asxml());
      })
  })
})


describe('P2X.JScanner', function(){
  describe('#lexall()', function(){
    it('it should return a token list, XML output option', function() {
        var scanner = P2X.JScanner();
        var tl = scanner.lexall();
        var xmlRes = "<scan-xml xmlns='http://johannes-willkomm.de/xml/code-xml/' "
            + "xmlns:ca='http://johannes-willkomm.de/xml/code-xml/attributes/'>\n"
            + '<input></input>\n'
            + '<ca:scanner-config>\n'
            + '</ca:scanner-config>\n'
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
                                       tokenName: undefined,
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
                                       tokenName: undefined,
                                       text: '111',
                                       index: 0,
                                       line: 1,
                                       col: 0 })
        assert.deepEqual(tl.list[1], { token: 112,
                                       tokenName: undefined,
                                       text: '11',
                                       index: 4,
                                       line: 1,
                                       col: 4 })
        assert.deepEqual(tl.list[2], { token: 111,
                                       tokenName: undefined,
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

  })
})


describe('P2X.Parser', function(){
  describe('#parse()', function(){
    var xmlres = fs.readFileSync('../examples/out/1p2ep3.xml')+''
    var xmlres2 = fs.readFileSync('../examples/out/l1p2r.xml')+''

    it('it should return an expression tree', function() {
        var scanner = P2X.JScanner()
        var scConf = [
            { re: '=',      action: TOKEN_EQUAL },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '[0-9]+', action: TOKEN_INTEGER },
        ]
        var input = '1+2=+3'
        scanner.set(scConf)
        scanner.str(input)
        var tl = scanner.lexall().mkeof()
        var parser = P2X.Parser()
        var parseConf = [
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
        ]
        parser.setconfig(parseConf)
        var res = parser.parse(tl)
        var tpOptions = P2X.TreePrinterOptions();
        var tp = P2X.TreePrinter(parser.tokenInfo, tpOptions)

        res = tp.asxml(parser.root)

        // console.log(res)
        assert.equal(res, xmlres);
    })

    it('the same with the easy-to-use function', function() {
        var scConf = [
            { re: '=',      action: TOKEN_EQUAL },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '[0-9]+', action: TOKEN_INTEGER },
        ]
        var input = '1+2=+3'
        var parseConf = [
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treeprinter: P2X.TreePrinterOptions()}
        res = P2X.p2xj(input, p2xConfig)

        // console.log(res)
        assert.equal(res, xmlres);
    })

    it('testing parentheses', function() {
        var scConf = [
            { re: '\\(',    action: TOKEN_L_PAREN },
            { re: '\\)',    action: TOKEN_R_PAREN },
            { re: '=',      action: TOKEN_EQUAL },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '\\*',    action: TOKEN_MULT },
            { re: '[0-9]+', action: TOKEN_INTEGER },
        ]
        var input = '(1+2)'
        var parseConf = [
            { type: TOKEN_L_PAREN, isParen: 1, closingList: [ { type: TOKEN_R_PAREN } ] },
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: TOKEN_MULT, mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treeprinter: P2X.TreePrinterOptions()}
        res = P2X.p2xj(input, p2xConfig)
        
        // console.log(res)
        assert.equal(res, xmlres2)
    })

    it('testing postfix op', function() {
        var xmlres = fs.readFileSync('../examples/out/ftp2.xml')+''
        var scConf = [
            { re: '\'',     action: TOKEN_QUOTE },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '\\*',    action: TOKEN_MULT },
            { re: '[0-9]+', action: TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: TOKEN_IDENTIFIER },
        ]
        var input = 'f\'+2'
        var parseConf = [
            { type: TOKEN_QUOTE, mode: MODE_POSTFIX, prec: 3000 },
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: TOKEN_MULT, mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treeprinter: P2X.TreePrinterOptions()}
        res = P2X.p2xj(input, p2xConfig)

        // console.log(res)
        assert.equal(res, xmlres)
    })

    it('testing postfix op (missing prec)', function() {
        var xmlres = fs.readFileSync('../examples/out/ftp2.xml')+''
        var scConf = [
            { re: '\'',     action: TOKEN_QUOTE },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '\\*',    action: TOKEN_MULT },
            { re: '[0-9]+', action: TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: TOKEN_IDENTIFIER },
        ]
        var input = 'f\'+2'
        var parseConf = [
            { type: TOKEN_QUOTE, mode: MODE_POSTFIX },
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: TOKEN_MULT, mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treeprinter: P2X.TreePrinterOptions()}
        res = P2X.p2xj(input, p2xConfig)

        // console.log(res)
        assert.equal(res, xmlres)
    })

    it('testing binary parenthesis', function() {
        var xmlres = fs.readFileSync('../examples/out/fl1rg.xml')+''
        var scConf = [
            { re: '\\(',    action: TOKEN_L_PAREN },
            { re: '\\)',    action: TOKEN_R_PAREN },
            { re: '=',      action: TOKEN_EQUAL },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '\\*',    action: TOKEN_MULT },
            { re: '[0-9]+', action: TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: TOKEN_IDENTIFIER },
        ]
        var input = 'f(1)g'
        var parseConf = [
            { type: TOKEN_L_PAREN, mode: MODE_BINARY, prec: 1050,
              isParen: 1, closingList: [ { type: TOKEN_R_PAREN } ] },
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: TOKEN_MULT, mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treeprinter: P2X.TreePrinterOptions()}
        res = P2X.p2xj(input, p2xConfig)

//        console.log(res)
        assert.equal(res, xmlres)
    })

    it('testing postfix parenthesis', function() {
        var xmlres = fs.readFileSync('../examples/out/f1.xml')+''
        var scConf = [
            { re: '\\(',    action: TOKEN_L_PAREN },
            { re: '\\)',    action: TOKEN_R_PAREN },
            { re: '=',      action: TOKEN_EQUAL },
            { re: '\\+',    action: TOKEN_PLUS },
            { re: '\\*',    action: TOKEN_MULT },
            { re: '[0-9]+', action: TOKEN_INTEGER },
            { re: '[a-zA-Z]+', action: TOKEN_IDENTIFIER },
        ]
        var input = 'f(1)'
        var parseConf = [
            { type: TOKEN_L_PAREN, mode: MODE_POSTFIX, prec: 3000, isParen: 1, closingList: [ { type: TOKEN_R_PAREN } ] },
            { type: TOKEN_EQUAL, mode: MODE_BINARY, assoc: ASSOC_RIGHT, prec: 500 },
            { type: TOKEN_PLUS, mode: MODE_UNARY_BINARY, assoc: ASSOC_LEFT, prec: 1000, precU: 2200 },
            { type: TOKEN_MULT, mode: MODE_BINARY, assoc: ASSOC_LEFT, prec: 1100 },
        ]
        var p2xConfig = {scanner: scConf, parser: parseConf, treeprinter: P2X.TreePrinterOptions()}
        res = P2X.p2xj(input, p2xConfig)

//        console.log(res)
        assert.equal(res, xmlres)
    })

  })
})

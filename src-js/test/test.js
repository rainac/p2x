
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
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>/abc/</re><action>1023</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          scConf = P2X.ScannerConfig().loadXML(xmlRes)
          assert.equal(xmlRes, scConf.asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>/abc/</re><action>1023</action></ca:lexem>\n'
              + '<ca:lexem><re>/abc/</re><action>TOKEN_IDENTIFIER</action></ca:lexem>\n'
              + '<ca:lexem><re>/abc/</re><action>TOKEN_PLUS</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          scConf = P2X.ScannerConfig().loadXML(xmlRes)
          assert.equal(xmlRes, scConf.asxml(''));
      })
      function setAndGetFromScanner(scConf) {
          console.dir(scConf)
          myScanner = P2X.JScanner();
          myScanner.set(scConf)
          console.dir(myScanner.actions)
          console.dir(myScanner.get())
          return myScanner.get()
      }
      function setAndGetFromScannerXML(inXML) {
          scConf = P2X.ScannerConfig().loadXML(inXml)
          return setAndGetFromScanner(scConf).asxml('')
      }
      it('should return XML rule list', function(){
          inXml = '<ca:scanner-config>\n'
              + '<ca:lexem><re>abc</re><action>1023</action></ca:lexem>\n'
              + '<ca:lexem><re>abc</re><action>11</action></ca:lexem>\n'
              + '<ca:lexem><re>abc</re><action>TOKEN_PLUS</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>abc</re><action>1023</action></ca:lexem>\n'
              + '<ca:lexem><re>abc</re><action>TOKEN_KEYW_FUNCTION</action></ca:lexem>\n'
              + '<ca:lexem><re>abc</re><action>TOKEN_PLUS</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          assert.equal(xmlRes, setAndGetFromScannerXML(inXml));
      })
      it('should return XML rule list', function(){
          inXml = '<ca:scanner-config>\n'
              + '<ca:lexem><re>(</re><action>TOKEN_MULT</action></ca:lexem>\n'
              + '<ca:lexem><re>\'</re><action>TOKEN_MINUS</action></ca:lexem>\n'
              + '<ca:lexem><re>\</re><action>TOKEN_MINUS</action></ca:lexem>\n'
              + '<ca:lexem><re>)</re><action>TOKEN_MINUS</action></ca:lexem>\n'
              + '<ca:lexem><re>&amp;</re><action>TOKEN_PLUS</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          assert.equal(inXml, setAndGetFromScannerXML(inXml));
      })
      function otstr(obj) {
          return JSON.stringify(obj);
      }
      it('set_get should return the same config', function(){
          scConf = [
              { re: 'abc', action: 49 },
              { re: '1+2*3?', action: 49 },
              ]
          assert.equal(otstr(scConf), otstr(setAndGetFromScanner(scConf)));
      })
      it('set_get should return the same config', function(){
          scConf = [
              { re: '(', action: 49 },
              { re: '\'', action: 48 },
              { re: '?', action: 48 },
              { re: ')', action: 48 },
              { re: '&', action: 47 }
              ]
          assert.equal(otstr(scConf), otstr(setAndGetFromScanner(scConf)));
      })
      it('empty RE should be rejected', function(){
          scConf = [
              { re: '', action: 48 },
              ]
          assert.equal('[]', otstr(setAndGetFromScanner(scConf)));
      })
      it('empty RE should be rejected', function(){
          scConf = [
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
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>abc</re><action>1023</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          scConf = [
              {re: /abc/, action: 1023},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>abc</re><action>TOKEN_DIV_EQUAL</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          scConf = [
              {re: /abc/, action: 100},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>a+b*c?</re><action>TOKEN_DOUBLE_LESS_EQUAL</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          action = 102
          scConf = [
              {re: /a+b*c?/, action: function() { return action }},
          ]
          assert.equal(xmlRes,
                       P2X.ScannerConfig(scConf).asxml(''));
      })
      it('should return XML rule list', function(){
          xmlRes = '<ca:scanner-config>\n'
              + '<ca:lexem><re>ab\\\\c\\b</re><action>function () { return action*2 }</action></ca:lexem>\n'
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
              + '<ca:lexem><re>a&lt;&gt;+b&lt;&amp;*c?</re><action>TOKEN_DOUBLE_LESS_EQUAL</action></ca:lexem>\n'
              + '</ca:scanner-config>\n'
          action = 102
          scConf = [
              {re: /a<>+b<&*c?/, action: function() { return action }},
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
                + '<ca:lexem><re>/abc/</re><action>function () { return action*2 }</action></ca:lexem>\n'
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



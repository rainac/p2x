/*
This file is part of P2X.
Copyright © 2014 Johannes Willkomm
See the file p2x.cc for copying conditions.
*/

#define P2X_NO_MAIN
#include "p2x.cc"

#include <stdlib.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

struct TokenTests  : public CppUnit::TestFixture {
  void setUp()
  {
  }
  void tearDown()
  {
  }

  void testTokenInit() {
    Token t;
    CPPUNIT_ASSERT(t.token == 0);
    CPPUNIT_ASSERT(t.left == 0);
    CPPUNIT_ASSERT(t.right == 0);
    CPPUNIT_ASSERT(t.ignore == 0);
    CPPUNIT_ASSERT(t.id == Token::count);
    CPPUNIT_ASSERT(t.line == 0);
    CPPUNIT_ASSERT(t.column == 0);
    CPPUNIT_ASSERT(t.character == 0);
    CPPUNIT_ASSERT(t.text == "");
  }

  void testTokenEqual() {
    TokenInfo tokenInfo;
    std::vector<Token*> tv;
    TokenList tl(tv);
    Parser p(tokenInfo, Parser::Options(), tl);

    tokenInfo.addBinary("and", 11);
    tokenInfo.addBinary("or", 11);

    {
      Token s(TOKEN_FLOAT, "1.234"), t(TOKEN_FLOAT, "1.2345");
      CPPUNIT_ASSERT(p.tokenTypeEqual(&s, &t));
    }

    {
      Token s(TOKEN_INTEGER, "1.234"), t(TOKEN_FLOAT, "1.2345");
      CPPUNIT_ASSERT(not p.tokenTypeEqual(&s, &t));
    }

    {
      Token s(TOKEN_STRING, "1.234"), t(TOKEN_STRING, "1.2345");
      CPPUNIT_ASSERT(p.tokenTypeEqual(&s, &t));
    }

    { // equal: both identifiers with no special types
      Token s(TOKEN_IDENTIFIER, "1.234"), t(TOKEN_IDENTIFIER, "1.2345");
      CPPUNIT_ASSERT(p.tokenTypeEqual(&s, &t));
    }

    { // not equal: both identifiers, but "and" has special type
      Token s(TOKEN_IDENTIFIER, "and"), t(TOKEN_IDENTIFIER, "1.2345");
      CPPUNIT_ASSERT(not p.tokenTypeEqual(&s, &t));
    }

    { // not equal: both identifiers, but "or" has special type
      Token s(TOKEN_IDENTIFIER, "1.234"), t(TOKEN_IDENTIFIER, "or");
      CPPUNIT_ASSERT(not p.tokenTypeEqual(&s, &t));
    }

    { // not equal: both identifiers, both special type, but different text
      Token s(TOKEN_IDENTIFIER, "and"), t(TOKEN_IDENTIFIER, "or");
      CPPUNIT_ASSERT(not p.tokenTypeEqual(&s, &t));
    }

    { // equal: both identifiers, both same special type, i.e. equal text
      Token s(TOKEN_IDENTIFIER, "and"), t(TOKEN_IDENTIFIER, "and");
      CPPUNIT_ASSERT(p.tokenTypeEqual(&s, &t));
    }

    { // equal: both identifiers, both same special type, i.e. equal text
      Token s(TOKEN_IDENTIFIER, "or"), t(TOKEN_IDENTIFIER, "or");
      CPPUNIT_ASSERT(p.tokenTypeEqual(&s, &t));
    }
  }

  CPPUNIT_TEST_SUITE( TokenTests );
  CPPUNIT_TEST( testTokenInit );
  CPPUNIT_TEST( testTokenEqual );
  CPPUNIT_TEST_SUITE_END();

};

struct ParseTests  : public CppUnit::TestFixture {
  void setUp()
  {
  }
  void tearDown()
  {
  }

  void doParseTest1(std::string exname, std::string flags = "") {
    // Test whether the parser still returns the same tree as those
    // created with older versions, and saved in examples/out
    std::string genCommand = "p2x " + flags + " -p ../examples/configs/default < ../examples/in/"
      + exname + ".exp > /tmp/" + exname + ".xml";
    std::cout << genCommand << "\n";
    std::string ppCommand = "xsltproc -o /tmp/" + exname + "2.xml ../src/xsl/but-root.xsl /tmp/" + exname + ".xml";
    std::string diffCommand = "diff ../examples/out/" + exname + ".xml /tmp/" + exname + "2.xml";
    std::string delCommand = "rm /tmp/" + exname + ".xml /tmp/" + exname + "2.xml";
    CPPUNIT_ASSERT(0 == system(genCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(ppCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(diffCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(delCommand.c_str()));
  }
  /*
  void doParseTest1latin1(std::string exname) {
    doParseTest1(exname, "-e iso88591");
  }

  void testParse0() {
    doParseTest1("1l1l1l1l1");
  }
  void testParse1() {
    doParseTest1("1r1r1r1r1");
  }
  void testParse2() {
    doParseTest1("1r1r1r1r1_mixed2");
  }
  void testParse3() {
    doParseTest1("1r1r1r1r1_mixed");
  }
  void testParse4() {
    doParseTest1("add3tadd3tadd3");
  }
  void testParse5() {
    doParseTest1("assign");
  }
  void testParse6() {
    doParseTest1("binary-minus");
  }
  void testParse7() {
    doParseTest1("binary-mult");
  }
  void testParse8() {
    doParseTest1("colon");
  }
  void testParse9() {
    doParseTest1("comma");
  }
  void testParse10() {
    doParseTest1("c_strings3");
  }
  void testParse11() {
    doParseTest1("div3");
  }
  void testParse12() {
    doParseTest1("easy.ec");
  }
  void testParse13() {
    doParseTest1("email-2");
  }
  void testParse14() {
    doParseTest1("email");
  }
  void testParse15() {
    doParseTest1("example1");
  }
  void testParse16() {
    doParseTest1("float");
  }
  void testParse17() {
    doParseTest1("full-ops");
  }
  void testParse18() {
    doParseTest1("general-linear");
  }
  void testParse19() {
    doParseTest1latin1("german");
  }
  void testParse20() {
    doParseTest1("german.utf8");
  }
  void testParse21() {
    doParseTest1("getenv.hh");
  }
  void testParse22() {
    doParseTest1("hello.c");
  }
  void testParse23() {
    doParseTest1("juxtasin");
  }
  void testParse24() {
    doParseTest1latin1("letter");
  }
  void testParse25() {
    doParseTest1("matlab_concat2");
  }
  void testParse26() {
    doParseTest1("matlab_concat3");
  }
  void testParse27() {
    doParseTest1("matlab_concat");
  }
  void testParse28() {
    doParseTest1("matlab-float");
  }
  void testParse29() {
    doParseTest1("matlab-linear-2");
  }
  void testParse30() {
    doParseTest1("matlab-linear");
  }
  void testParse31() {
    doParseTest1("matlab_strings2");
  }
  void testParse32() {
    doParseTest1("matlab_strings3");
  }
  void testParse33() {
    doParseTest1("matlab_strings4");
  }
  void testParse34() {
    doParseTest1("matlab_strings4_f");
  }
  void testParse35() {
    doParseTest1("matlab_strings4_f.m");
  }
  void testParse36() {
    doParseTest1("matlab_strings");
  }
  void testParse37() {
    doParseTest1("matlab-strings");
  }
  void testParse38() {
    doParseTest1("matlab_strings.m");
  }
  void testParse39() {
    doParseTest1("miss-beg");
  }
  void testParse40() {
    doParseTest1("miss-end");
  }
  void testParse41() {
    doParseTest1("mult3");
  }
  void testParse42() {
    doParseTest1("mult3+mult3");
  }
  void testParse43() {
    doParseTest1("mult3+mult3+mult3");
  }
  void testParse44() {
    doParseTest1("multsin");
  }
  void testParse45() {
    doParseTest1("no-ops");
  }
  void testParse46() {
    doParseTest1("octave_function.m");
  }
  void testParse47() {
    doParseTest1("octave-function.m");
  }
  void testParse48() {
    doParseTest1("octave_if.m");
  }
  void testParse49() {
    doParseTest1("octave_strings");
  }
  void testParse50() {
    doParseTest1("octave_strings.m");
  }
  void testParse51() {
    doParseTest1("one");
  }
  void testParse52() {
    doParseTest1("onetwo");
  }
  void testParse53() {
    doParseTest1("oplist");
  }
  void testParse54() {
    doParseTest1("par1");
  }
  void testParse55() {
    doParseTest1("par2");
  }
  void testParse56() {
    doParseTest1("plus3");
  }
  void testParse57() {
    doParseTest1("plus");
  }
  void testParse58() {
    doParseTest1("plusmult");
  }
  void testParse59() {
    doParseTest1("plussin");
  }
  void testParse60() {
    doParseTest1("polynom-1");
  }
  void testParse61() {
    doParseTest1("postfix_test2");
  }
  void testParse62() {
    doParseTest1("postfix_test");
  }
  void testParse63() {
    doParseTest1("prec-ascendingdz");
  }
  void testParse64() {
    doParseTest1("prec-ascending");
  }
  void testParse65() {
    doParseTest1("prec-ascendingez");
  }
  void testParse66() {
    doParseTest1("prec-ascendingtz");
  }
  void testParse67() {
    doParseTest1("prec-ascending+z");
  }
  void testParse68() {
    doParseTest1("prec-falling");
  }
  void testParse69() {
    doParseTest1("randexpr1_1");
  }
  void testParse70() {
    doParseTest1("randexpr1_2");
  }
  void testParse71() {
    doParseTest1("randexpr1_3");
  }
  void testParse72() {
    doParseTest1("randexpr1_4");
  }
  void testParse73() {
    doParseTest1("randexpr1_5");
  }
  void testParse74() {
    doParseTest1("randexpr1_6");
  }
  void testParse75() {
    doParseTest1("randexpr2_1");
  }
  void testParse76() {
    doParseTest1("randexpr2_2");
  }
  void testParse77() {
    doParseTest1("randexpr2_3");
  }
  void testParse78() {
    doParseTest1("randexpr2_4");
  }
  void testParse79() {
    doParseTest1("randexpr2_5");
  }
  void testParse80() {
    doParseTest1("randexpr2_6");
  }
  void testParse81() {
    doParseTest1("randexpr3_1");
  }
  void testParse82() {
    doParseTest1("randexpr3_2");
  }
  void testParse83() {
    doParseTest1("randexpr3_3");
  }
  void testParse84() {
    doParseTest1("randexpr3_4");
  }
  void testParse85() {
    doParseTest1("randexpr3_5");
  }
  void testParse86() {
    doParseTest1("randexpr3_6");
  }
  void testParse87() {
    doParseTest1("reply");
  }
  void testParse88() {
    doParseTest1("rfc822-header");
  }
  void testParse89() {
    doParseTest1("semi");
  }
  void testParse90() {
    doParseTest1("sentence-2");
  }
  void testParse91() {
    doParseTest1("sentence");
  }
  void testParse92() {
    doParseTest1("test");
  }
  void testParse93() {
    doParseTest1("unary");
  }
  void testParse94() {
    doParseTest1("unary-minus1");
  }
  void testParse95() {
    doParseTest1("xml");
  }
  void testParse96() {
    doParseTest1("binary-mult");
  }
  void testParse97() {
    doParseTest1("commaplus");
  }
  void testParse98() {
    doParseTest1("commasemicolon");
  }
  void testParse99() {
    doParseTest1("postfix1");
  }
  void testParse100() {
    doParseTest1("postfix2");
  }
  void testParse101() {
    doParseTest1("postfix3");
  }
  void testParse102() {
    doParseTest1("print1");
  }
  void testParse103() {
    doParseTest1("print2");
  }
  void testParse104() {
    doParseTest1("unary-minus2");
  }
  */

  void doParseMergedNewline(std::string exname, std::string flags = "") {
    // Test whether the "merged" output for a node yields identical
    // results to merging that node a posteriori

    std::string genCommand = "p2x --indent " + flags + " -p ../examples/configs/default < ../examples/in/"
      + exname + ".exp > /tmp/" + exname + ".xml";
    std::string ppCommand = "xsltproc -o /tmp/" + exname + "2.xml ../src/xsl/but-root.xsl /tmp/" + exname + ".xml";
    std::string ppCommand2 = "xsltproc -o /tmp/" + exname + "3.xml ../src/xsl/merge-newlines.xsl /tmp/" + exname + "2.xml";

    std::cout << genCommand << "\n";
    std::cout << ppCommand << "\n";
    std::cout << ppCommand2 << "\n";
    CPPUNIT_ASSERT(0 == system(genCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(ppCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(ppCommand2.c_str()));

    genCommand = "p2x --indent " + flags + " -p ../examples/configs/newline-merged < ../examples/in/"
      + exname + ".exp > /tmp/" + exname + ".xml";
    ppCommand = "xsltproc -o /tmp/" + exname + "2.xml ../src/xsl/but-root.xsl /tmp/" + exname + ".xml";

    std::cout << genCommand << "\n";
    std::cout << ppCommand << "\n";
    CPPUNIT_ASSERT(0 == system(genCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(ppCommand.c_str()));

    std::string diffCommand = "diff /tmp/" + exname + "2.xml /tmp/" + exname + "3.xml";

    std::string delCommand = "rm /tmp/" + exname + ".xml /tmp/" + exname + "2.xml /tmp/" + exname + "3.xml";

    std::cout << diffCommand << "\n";
    std::cout << delCommand << "\n";
    CPPUNIT_ASSERT(0 == system(diffCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(delCommand.c_str()));
  }

  void testParse200() {
    doParseMergedNewline("unary-minus2");
  }

  void testParse201() {
    doParseMergedNewline("float");
  }

  CPPUNIT_TEST_SUITE( ParseTests );
  /*
  CPPUNIT_TEST( testParse0 );
  CPPUNIT_TEST( testParse1 );
  CPPUNIT_TEST( testParse2 );
  CPPUNIT_TEST( testParse3 );
  CPPUNIT_TEST( testParse4 );
  CPPUNIT_TEST( testParse5 );
  CPPUNIT_TEST( testParse6 );
  CPPUNIT_TEST( testParse7 );
  CPPUNIT_TEST( testParse8 );
  CPPUNIT_TEST( testParse9 );
  CPPUNIT_TEST( testParse10 );
  CPPUNIT_TEST( testParse11 );
  CPPUNIT_TEST( testParse12 );
  CPPUNIT_TEST( testParse13 );
  CPPUNIT_TEST( testParse14 );
  CPPUNIT_TEST( testParse15 );
  CPPUNIT_TEST( testParse16 );
  CPPUNIT_TEST( testParse17 );
  CPPUNIT_TEST( testParse18 );
  CPPUNIT_TEST( testParse19 );
  CPPUNIT_TEST( testParse20 );
  CPPUNIT_TEST( testParse21 );
  CPPUNIT_TEST( testParse22 );
  CPPUNIT_TEST( testParse23 );
  CPPUNIT_TEST( testParse24 );
  CPPUNIT_TEST( testParse25 );
  CPPUNIT_TEST( testParse26 );
  CPPUNIT_TEST( testParse27 );
  CPPUNIT_TEST( testParse28 );
  CPPUNIT_TEST( testParse29 );
  CPPUNIT_TEST( testParse30 );
  CPPUNIT_TEST( testParse31 );
  CPPUNIT_TEST( testParse32 );
  CPPUNIT_TEST( testParse33 );
  CPPUNIT_TEST( testParse34 );
  CPPUNIT_TEST( testParse35 );
  CPPUNIT_TEST( testParse36 );
  CPPUNIT_TEST( testParse37 );
  CPPUNIT_TEST( testParse38 );
  CPPUNIT_TEST( testParse39 );
  CPPUNIT_TEST( testParse40 );
  CPPUNIT_TEST( testParse41 );
  CPPUNIT_TEST( testParse42 );
  CPPUNIT_TEST( testParse43 );
  CPPUNIT_TEST( testParse44 );
  CPPUNIT_TEST( testParse45 );
  CPPUNIT_TEST( testParse46 );
  CPPUNIT_TEST( testParse47 );
  CPPUNIT_TEST( testParse48 );
  CPPUNIT_TEST( testParse49 );
  CPPUNIT_TEST( testParse50 );
  CPPUNIT_TEST( testParse51 );
  CPPUNIT_TEST( testParse52 );
  CPPUNIT_TEST( testParse53 );
  CPPUNIT_TEST( testParse54 );
  CPPUNIT_TEST( testParse55 );
  CPPUNIT_TEST( testParse56 );
  CPPUNIT_TEST( testParse57 );
  CPPUNIT_TEST( testParse58 );
  CPPUNIT_TEST( testParse59 );
  CPPUNIT_TEST( testParse60 );
  CPPUNIT_TEST( testParse61 );
  CPPUNIT_TEST( testParse62 );
  CPPUNIT_TEST( testParse63 );
  CPPUNIT_TEST( testParse64 );
  CPPUNIT_TEST( testParse65 );
  CPPUNIT_TEST( testParse66 );
  CPPUNIT_TEST( testParse67 );
  CPPUNIT_TEST( testParse68 );
  CPPUNIT_TEST( testParse69 );
  CPPUNIT_TEST( testParse70 );
  CPPUNIT_TEST( testParse71 );
  CPPUNIT_TEST( testParse72 );
  CPPUNIT_TEST( testParse73 );
  CPPUNIT_TEST( testParse74 );
  CPPUNIT_TEST( testParse75 );
  CPPUNIT_TEST( testParse76 );
  CPPUNIT_TEST( testParse77 );
  CPPUNIT_TEST( testParse78 );
  CPPUNIT_TEST( testParse79 );
  CPPUNIT_TEST( testParse80 );
  CPPUNIT_TEST( testParse81 );
  CPPUNIT_TEST( testParse82 );
  CPPUNIT_TEST( testParse83 );
  CPPUNIT_TEST( testParse84 );
  CPPUNIT_TEST( testParse85 );
  CPPUNIT_TEST( testParse86 );
  CPPUNIT_TEST( testParse87 );
  CPPUNIT_TEST( testParse88 );
  CPPUNIT_TEST( testParse89 );
  CPPUNIT_TEST( testParse90 );
  CPPUNIT_TEST( testParse91 );
  CPPUNIT_TEST( testParse92 );
  CPPUNIT_TEST( testParse93 );
  CPPUNIT_TEST( testParse94 );
  CPPUNIT_TEST( testParse95 );
  CPPUNIT_TEST( testParse96 );
  CPPUNIT_TEST( testParse97 );
  CPPUNIT_TEST( testParse98 );
  CPPUNIT_TEST( testParse99 );
  CPPUNIT_TEST( testParse100 );
  CPPUNIT_TEST( testParse101 );
  CPPUNIT_TEST( testParse102 );
  CPPUNIT_TEST( testParse103 );
  CPPUNIT_TEST( testParse104 );
  */

  CPPUNIT_TEST( testParse200 );
  CPPUNIT_TEST( testParse201 );
  CPPUNIT_TEST_SUITE_END();

};


struct ConfigTests  : public CppUnit::TestFixture {
  void setUp() { }
  void tearDown() { }

  void doConfigTestFail(std::string const &configFile, std::string const &exname = "one", std::string flags = "") {
    // Test whether p2x exists with error upon reading configFile
    std::string genCommand = "p2x " + flags + " -p ../examples/configs/" + configFile + " < ../examples/in/"
      + exname + ".exp > /tmp/" + exname + ".xml 2> /tmp/" + exname + ".err";
    std::string delCommand = "rm /tmp/" + exname + ".xml /tmp/" + exname + ".err";
    CPPUNIT_ASSERT(0 != system(genCommand.c_str()));
    CPPUNIT_ASSERT(0 == system(delCommand.c_str()));
  }

  void testConfigOverrideToParen() {
    doConfigTestFail("overwrite-to-paren");
  }

  CPPUNIT_TEST_SUITE( ConfigTests );
  CPPUNIT_TEST( testConfigOverrideToParen );
  CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( TokenTests );
CPPUNIT_TEST_SUITE_REGISTRATION( ParseTests );
CPPUNIT_TEST_SUITE_REGISTRATION( ConfigTests );

int main( int , char **)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run( "", false );
  return !wasSuccessful;
}

// Local Variables:
// coding: utf-8
// End:

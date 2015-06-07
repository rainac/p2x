#! /bin/bash

#set -x

checkParseTreeEqual() {
    infile=$1
    exp=$2
    opts="$3"
    p2x $opts -p ../../examples/configs/default ../../examples/in/$infile > res.xml
    xsltproc -o res.txt ../../src/xsl/parens.xsl res.xml
    res=$(cat res.txt)
    echo "$infile: $(cat ../../examples/in/$infile)  =>  $res"
    assertEquals "Parse tree is not in expected form: '$exp' != '$res'" "$exp" "$res"
}

testParseTreeEqual1() {
    checkParseTreeEqual postfix-test.exp "[NEWLINE]([a]([P](2), 3))"
}

testParseTreeEqual2() {
    checkParseTreeEqual mult3.exp "[MULT]([MULT](2, 3), 4)"
}

testParseTreeEqual3() {
    checkParseTreeEqual assign.exp "[EQUAL](x, [EQUAL](y, [EQUAL](z, 16)))"
}

testParseTreeEqual4() {
    checkParseTreeEqual multsin.exp "[MULT]([MULT]([sin](., 2), [sin](., 3)), [sin](., 4))"
}

testParseTreeEqual5() {
    checkParseTreeEqual binary-prec.exp "[a](1, [b](2, [c](3, 4)))"
}

testParseTreeEqual6() {
    checkParseTreeEqual binary-prec2.exp "[a]([b]([c](1, 2), 3), 4)"
}

testParseTreeEqual7() {
    # unary vs. binary with lower prec
    checkParseTreeEqual unary-prec.exp "[b]([ueqc](., 2), 3)"
}

testParseTreeEqual8() {
    # unary vs. binary with higher prec
    checkParseTreeEqual unary-prec2.exp "[ueqc](., [d](2, 3))"
}

testParseTreeEqual9() {
    # see what happens when unary hits binary with same prec
    checkParseTreeEqual unary-prec3.exp "[c]([ueqc](., 2), 3)"
}

testParseTreeEqual10() {
    checkParseTreeEqual unary2.exp "[a](1, [ueqc](., 2))"
}

testParseTreeEqual11() {
    # unary operator between two items -> JUXTA inserted
    checkParseTreeEqual unary3.exp "[JUXTA](1, [ueqc](., 2))"
}

testParseTreeEqual12() {
    checkParseTreeEqual unary-binary.exp "[MINUS](1, 2)"
}

testParseTreeEqual13() {
    checkParseTreeEqual unary-binary2.exp "[a](1, [MINUS](., 2))"
}

testParseTreeEqual14() {
    checkParseTreeEqual unary-binary3.exp "[MINUS]([P](1), 2)"
}

testParseTreeEqual14() {
    checkParseTreeEqual unary-binary4.exp "[a](1, [ub](., 2))"
}

testParseTreeEqual15() {
    checkParseTreeEqual unary-binary5.exp "[e](1, [ub](., 2))"
}

testParseTreeEqual_ub_6() {
    checkParseTreeEqual unary-binary6.exp "[a](1, [b]([ub](., 2), 3))"
}
testParseTreeEqual_ub_7() {
    checkParseTreeEqual unary-binary7.exp "[a](1, [ub](., [d](2, 3)))"
}
testParseTreeEqual_ub_8() {
    checkParseTreeEqual unary-binary8.exp "[b]([e](1, [ub](., 2)), 3)"
}
testParseTreeEqual_ub_9() {
    checkParseTreeEqual unary-binary9.exp "[d]([e](1, [ub](., 2)), 3)"
}
testParseTreeEqual_ub_10() {
    checkParseTreeEqual unary-binary10.exp "[e](1, [ub](., [f](2, 3)))"
}

testParseTreeEqual_r_1() {
    checkParseTreeEqual right.exp "[r](1, [r](2, [s](3, [s](4, 5))))"
}
testParseTreeEqual_r_2() {
    checkParseTreeEqual right2.exp "[r]([s](1, [s](2, 3)), [r](4, 5))"
}

testParseTreeEqual_p_1() {
    checkParseTreeEqual postfix-low.exp "[p]([a](1, 2))"
}
testParseTreeEqual_p_2() {
    checkParseTreeEqual postfix-high.exp "[a](1, [P](2))"
}

testParseTreeEqual_word_par_1() {
    # test words as paren delimiters
    checkParseTreeEqual word-paren.exp "[MULT](1, [open](., [PLUS](2, 3)))"
}
testParseTreeEqual_word_par_2() {
    # test words as paren delimiters, with alternative endings
    checkParseTreeEqual word-paren2.exp "[MULT](1, [begin](., [PLUS](2, 3)))"
}
testParseTreeEqual_word_par_3() {
    # test words as paren delimiters, with alternative endings
    checkParseTreeEqual word-paren3.exp "[MULT](1, [begin](., [PLUS](2, 3)))"
}
testParseTreeEqual_word_par_4() {
    # test empty word paren
    checkParseTreeEqual word-paren-empty.exp "[JUXTA]([MULT](1, ftest), [open]())"
}

testParseTreeEqual_bin_par_1() {
    checkParseTreeEqual bin-paren.exp "[bopen](1, [PLUS](2, 3), 4)" "-i NEWLINE"
}

testParseTreeEqual_bin_par_2() {
    checkParseTreeEqual bin-paren-empty.exp "[bopen](1, 2)" "-i NEWLINE"
    checkParseTreeEqual bin-paren-empty.exp "[bopen](1, ., 2)" "-i NEWLINE --strict"
}

testParseTreeEqual_un_par_1() {
    checkParseTreeEqual un-paren.exp "[MULT](1, [uopen](., [PLUS](2, 3), 4))" "-i NEWLINE"
}

testParseTreeEqual_un_par_2() {
    checkParseTreeEqual un-paren-empty.exp "[MULT](1, [uopen](., 2))" "-i NEWLINE"
    checkParseTreeEqual un-paren-empty.exp "[MULT](1, [uopen](., ., 2))" "-i NEWLINE --strict"
}

testParseTreeEqual_post_par_1() {
    checkParseTreeEqual post-paren.exp "[MULT]([popen](1, [PLUS](2, 3)), 4)" "-i NEWLINE"
}

testParseTreeEqual_post_par_2() {
    checkParseTreeEqual post-paren-empty.exp "[MULT]([popen](1), 2)" "-i NEWLINE"
    checkParseTreeEqual post-paren-empty.exp "[MULT]([popen](1), 2)" "-i NEWLINE --strict"
}

testParseTreeEqual_post_par_3() {
    checkParseTreeEqual post-paren2.exp "[popen](xx, [PLUS](2, 3))" "-i NEWLINE"
}

testParseTreeEqual_post_par_4() {
    checkParseTreeEqual post-paren3.exp "[PLUS]([popen](xx, [PLUS](2, 3)), 4)" "-i NEWLINE"
}

testParseTreeEqual_post_par_4() {
    checkParseTreeEqual post-paren4.exp "[PLUS]([popen](xx, [PLUS](2, 3)), 4)" "-i NEWLINE"
}

testParseTreeEqual_unary_binary11() {
    checkParseTreeEqual unary-binary11.exp "[PLUS]([PLUS](., 1), 2)"
}

testParseTreeEqual_unary_binary12() {
    checkParseTreeEqual unary-binary12.exp "[MINUS]([MINUS](., 1), 2)"
}

testParseTreeEqual_unary_binary13() {
    checkParseTreeEqual unary-binary13.exp "[NEWLINE]([PLUS]([MULT](1), 2), [MULT](1, [MINUS](., 2)))"
}

testParseTreeEqual_noclose() {
    checkParseTreeEqual noclose.exp "[COMMA](int, [L_PAREN](., [NEWLINE]()))"
}

testParseTreeEqual_noclose2() {
    checkParseTreeEqual noclose2.exp "[COMMA](int, [open](., [NEWLINE](3)))"
}

testParseTreeEqual_noclose3() {
    checkParseTreeEqual noclose3.exp "[COMMA](int, [open](., [begin](., 3)))"
}

. shunit2

#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup.sh

testParseTreeEqual1() {
    checkParseTreeEqual postfix-test.exp "[NEWLINE]([a]([P](2), 3))"
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
    checkParseTreeEqual unary-prec3.exp "[b]([ueqc](., 2), 3)"
}

testParseTreeEqual9a() {
    # see what happens when unary follows binary with higher prec
    checkParseTreeEqual unary-prec4.exp "[e](2, [ueqc](., 4))"
}

testParseTreeEqual9a() {
    # see what happens when unary follows binary with higher prec
    checkParseTreeEqual unary-prec4.exp "[e](2, [ueqc](., 4))"
}

testParseTreeEqual9b() {
    # see what happens when unary follows binary with higher prec, and
    # is then followed by a binary with higher prec
    echo this test does not work in the current master
    checkParseTreeEqual unary-prec5.exp "[e](2, [ueqc](., [d](3, 4)))"
}

testParseTreeEqual9c() {
    # see what happens when unary follows binary with higher prec, and
    # is then followed by a binary with lower prec
    checkParseTreeEqual unary-prec6.exp "[a]([e](2, [ueqc](., 3)), 4)"
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
    checkParseTreeEqual unary-binary9.exp "[e](1, [ub](., [d](2, 3)))"
}
testParseTreeEqual_ub_10() {
    checkParseTreeEqual unary-binary10.exp "[e](1, [ub](., [f](2, 3)))"
}

testParseTreeEqual_p_1() {
    checkParseTreeEqual postfix-low.exp "[p]([a](1, 2))"
}
testParseTreeEqual_p_2() {
    checkParseTreeEqual postfix-high.exp "[a](1, [P](2))"
}

testParseTreeEqual_plain_paren() {
    checkParseTreeEqual paren.exp "[MULT]([L_PAREN](., [R_PAREN]([PLUS](1, 2))), 3)"
}

testParseTreeEqual_word_par_1() {
    # test words as paren delimiters
    checkParseTreeEqual word-paren.exp "[MULT](1, [open](., [close]([PLUS](2, 3))))"
}
testParseTreeEqual_word_par_2() {
    # test words as paren delimiters, with alternative endings
    checkParseTreeEqual word-paren2.exp "[MULT](1, [begin](., [finish]([PLUS](2, 3))))"
}
testParseTreeEqual_word_par_3() {
    # test words as paren delimiters, with alternative endings
    checkParseTreeEqual word-paren3.exp "[MULT](1, [begin](., [endblock]([PLUS](2, 3))))"
}
testParseTreeEqual_word_par_4() {
    # test empty word paren
    checkParseTreeEqual word-paren-empty.exp "[JUXTA]([MULT](1, ftest), [open](., [close]()))"
}

testParseTreeEqual_bin_par_1() {
    checkParseTreeEqual bin-paren.exp "[bopen](1, [bclose]([PLUS](2, 3), 4))" "-i NEWLINE"
}

testParseTreeEqual_bin_par_2() {
    checkParseTreeEqual bin-paren-empty.exp "[bopen](1, [bclose](., 2))" "-i NEWLINE"
    checkParseTreeEqual bin-paren-empty.exp "[bopen](1, [bclose](., 2))" "-i NEWLINE --strict"
}

testParseTreeEqual_bin_par_3() {
    checkParseTreeEqual bin-paren-prec.exp "[b]([bopen](1, [bclose]([PLUS](2, 3), 4)), 5)" "-i NEWLINE"
    checkParseTreeEqual bin-paren-prec.exp "[b]([bopen](1, [bclose]([PLUS](2, 3), 4)), 5)" "-i NEWLINE --strict"
}

testParseTreeEqual_bin_par_4() {
    checkParseTreeEqual bin-paren-prec2.exp "[bopen](1, [bclose]([PLUS](2, 3), [d](4, 5)))" "-i NEWLINE"
    checkParseTreeEqual bin-paren-prec2.exp "[bopen](1, [bclose]([PLUS](2, 3), [d](4, 5)))" "-i NEWLINE --strict"
}

testParseTreeEqual_un_par_1() {
    checkParseTreeEqual un-paren.exp "[MULT](1, [uopen](., [uclose]([PLUS](2, 3), 4)))" "-i NEWLINE"
}

testParseTreeEqual_un_par_2() {
    checkParseTreeEqual un-paren-empty.exp "[MULT](1, [uopen](., [uclose](., 2)))" "-i NEWLINE"
    checkParseTreeEqual un-paren-empty.exp "[MULT](1, [uopen](., [uclose](., 2)))" "-i NEWLINE --strict"
}

testParseTreeEqual_post_par_1() {
    checkParseTreeEqual post-paren.exp "[MULT]([popen](1, [pclose]([PLUS](2, 3))), 4)" "-i NEWLINE"
}

testParseTreeEqual_post_par_2() {
    checkParseTreeEqual post-paren-empty.exp "[MULT]([popen](1, [pclose]()), 2)" "-i NEWLINE"
    checkParseTreeEqual post-paren-empty.exp "[MULT]([popen](1, [pclose]()), 2)" "-i NEWLINE --strict"
}

testParseTreeEqual_post_par_3() {
    checkParseTreeEqual post-paren2.exp "[popen](xx, [pclose]([PLUS](2, 3)))" "-i NEWLINE"
}

testParseTreeEqual_post_par_4() {
    checkParseTreeEqual post-paren3.exp "[PLUS]([popen](xx, [pclose]([PLUS](2, 3))), 4)" "-i NEWLINE"
}

testParseTreeEqual_post_par_4() {
    checkParseTreeEqual post-paren4.exp "[PLUS]([popen](xx, [pclose]([PLUS](2, 3))), 4)" "-i NEWLINE"
}

testParseTreeEqual_post_par_5() {
    checkParseTreeEqual post-paren-prec1.exp "[b]([popen](xx, [pclose]([PLUS](2, 3))), 4)" "-i NEWLINE"
}

testParseTreeEqual_post_par_6() {
    checkParseTreeEqual post-paren-prec2.exp "[d]([popen](xx, [pclose]([PLUS](2, 3))), 4)" "-i NEWLINE"
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

testParseTreeEqual_binary_incrp() {
    checkParseTreeEqual incrp.exp "[PLUS](1, [MULT](2, [POW](3, 4)))"
}

testParseTreeEqual_unary_binary_valid() {
    checkParseTreeEqual ub.exp "[ub](1, [ub](., 2))"
}

testParseTreeEqual_stray_parenclose_in_expr() {
    checkParseTreeEqual stray-paren-close.exp "[popen](f1, [pclose]([PLUS](1, [MULT]([R_PAREN](), 2))))"
}

testParseTreeEqual_stray_opparenclose_in_expr() {
    checkParseTreeEqual stray-opparen-close.exp "[popen](f1, [pclose]([bclose](1, 2)))"
}

. ./myshunit2

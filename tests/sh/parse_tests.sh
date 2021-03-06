#! /bin/bash

#set -x

checkParseTreeEqual() {
    infile=$1
    exp=$2
    opts="$3"
    config="${4:-../../examples/configs/default}"
    p2x $opts -p $config ../../examples/in/$infile > res.xml
    xsltproc -o res.txt ../../src/xsl/parens.xsl res.xml
    res=$(cat res.txt)
    echo "$infile: $(cat ../../examples/in/$infile)  =>  $res"
    if [[ "$exp" != "$res" ]]; then
        echo "**** soll ****"
        echo $exp
        echo "**** result ****"
        echo $res
        echo "****  ****"
    fi
    assertEquals "Parse tree is not in expected form" "$exp" "$res"
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
#    checkParseTreeEqual unary-prec5.exp "[e](2, [ueqc](., [d](3, 4)))"
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

testParseTreeEqual_binary_incrp() {
    checkParseTreeEqual incrp.exp "[PLUS](1, [MULT](2, [POW](3, 4)))"
}

testParseTreeEqual_question_mark() {
    # see what happens when unary follows binary with higher prec
    checkParseTreeEqual question.exp "[EQUAL]([JUXTA](Is, [MULT]([MULT](2, 2), 2)), [QUESTION](7))"
}

testParseTreeEqual_low_prec_binary_seps() {
    checkParseTreeEqual low-prec-binary-seps.exp "[PLUS]([PLUS](1, 3), 5)"
}

testParseTreeEqual_low_prec_binary_seps2() {
    checkParseTreeEqual low-prec-binary-seps2.exp "[PLUS]([PLUS]([PLUS](1, 3), [DIV](5, 3)), [MULT](1, [DIV](2, 6)))"
}

testParseTreeEqual_low_prec_binary_seps3() {
    checkParseTreeEqual low-prec-binary-seps3.exp "[PLUS]([PLUS](1, 3), 5)"
}

testParseTreeEqual_juxta_binary() {
    checkParseTreeEqual juxta-binary.exp "[PLUS]([PLUS](1, 3), [MULT](., 5))"
}

testParseTreeEqual_juxta_binary2() {
    checkParseTreeEqual juxta-binary2.exp "[PLUS]([PLUS](1, [MULT](3)), 5)"
}

testParseTreeEqual_matlab_floats() {
    checkParseTreeEqual floatmatlab.exp "[NEWLINE]([DOTMULT](12., 13.4), [DOTMULT](12, 13.4), [DOTBACKSLASH](12, 13.4I), [DOTDIV](12, 13.4), [DOTPOW](12, 13.4))" "-S m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_matlab_strings() {
    checkParseTreeEqual stringsmatlab.exp "[NEWLINE]([L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([APOS](a), [APOS](b)), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]('sd', 'sd'), 'sd'))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA](a, ' b'), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [APOS]([L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c)))))), [APOS](d)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), ' c '), ' d '))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c))))), ' d'))))" "-S m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_matlab_floats_re2c() {
    checkParseTreeEqual floatmatlab.exp "[NEWLINE]([DOTMULT](12., 13.4), [DOTMULT](12, 13.4), [DOTBACKSLASH](12, 13.4I), [DOTDIV](12, 13.4), [DOTPOW](12, 13.4))" "-S re2c_m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_matlab_strings_re2c() {
    checkParseTreeEqual stringsmatlab.exp "[NEWLINE]([L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([APOS](a), [APOS](b)), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]('sd', 'sd'), 'sd'))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA](a, ' b'), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [APOS]([L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c)))))), [APOS](d)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), ' c '), ' d '))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c))))), ' d'))))" "-S re2c_m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_linecomment_newline() {
    checkParseTreeEqual lncnl.exp "[NEWLINE](test, test)"
}

testParseTreeEqual_linecomment_newline2() {
    checkParseTreeEqual lncnl2.exp "[NEWLINE]([NEWLINE](test, test))"
}

testParseTreeEqual_block_closed_in_comment() {
    checkParseTreeEqual opencloseincomment.exp "[begin](., [NEWLINE]([NEWLINE]([NEWLINE](., [begin](., [endblock]([NEWLINE]([NEWLINE](., [PLUS](x, 1)))))), [PLUS](y, 1))))"
}

testParseTreeEqual_block_comment1() {
    checkParseTreeEqual ccomment.exp '[NEWLINE]([SEMICOLON]([EQUAL]([JUXTA](char, [MULT](const, [s]())), [JUXTA]("Hi", " she said"))))' "-S c"
}

testParseTreeEqual_block_comment2() {
    checkParseTreeEqual word-paren-comment.exp '[NEWLINE]([SEMICOLON]([EQUAL]([JUXTA](char, [MULT](const, [s]())), [JUXTA]("Hi", " she said"))))'
}

testParseTreeEqual_block_comment3() {
    checkParseTreeEqual word-paren-comment-nested.exp '[NEWLINE]([SEMICOLON]([EQUAL]([JUXTA](char, [MULT](const, [s]())), [JUXTA]("Hi", " it said\n"))))'
}

testParseTreeEqual_block_comment4() {
    checkParseTreeEqual word-paren-comment-unclosed.exp '[EQUAL]([JUXTA](char, [MULT](const, [s]())), "Hi")'
}

. shunit2

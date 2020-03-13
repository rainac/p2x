#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup_zsh.sh

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
  env -u P2XFLAGS ./parse_tests_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual2() {
  env -u P2XFLAGS  ./parse_tests_not_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual3() {
  env P2XFLAGS=-m  ./parse_tests_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual4() {
  env -u P2XFLAGS  ./parse_tests_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual5() {
  env -u P2XFLAGS ./parse_tests_null_placement.sh
  assertEquals "sub test should pass" "0" "$?"
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
    checkParseTreeEqual floatmatlab.exp "[NEWLINE]([DOTMULT](12., 13.4), [DOTMULT](12, 13.4), [DOTBACKSLASH](12, 13.4I), [DOTDIV](12, 13.4), [DOTPOW](12, 13.4), .)" "-S m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_matlab_strings() {
    checkParseTreeEqual stringsmatlab.exp "[NEWLINE]([L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([APOS](a), [APOS](b)), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]('sd', 'sd'), 'sd'))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA](a, ' b'), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [APOS]([L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c)))))), [APOS](d)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), ' c '), ' d '))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c))))), ' d'))), .)" "-S m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_matlab_floats_re2c() {
    checkParseTreeEqual floatmatlab.exp "[NEWLINE]([DOTMULT](12., 13.4), [DOTMULT](12, 13.4), [DOTBACKSLASH](12, 13.4I), [DOTDIV](12, 13.4), [DOTPOW](12, 13.4), .)" "-S re2c_m" ../../examples/configs-special/matlab.p2c
}

testParseTreeEqual_matlab_strings_re2c() {
    checkParseTreeEqual stringsmatlab.exp "[NEWLINE]([L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([APOS](a), [APOS](b)), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]('sd', 'sd'), 'sd'))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA](a, ' b'), [APOS](c)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [APOS]([L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c)))))), [APOS](d)))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), ' c '), ' d '))), [L_BRACKET](., [R_BRACKET]([JUXTA]([JUXTA]([JUXTA](a, ' b '), [L_PAREN](., [R_PAREN]([L_BRACKET](., [R_BRACKET](c))))), ' d'))), .)" "-S re2c_m" ../../examples/configs-special/matlab.p2c
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

. ./myshunit2

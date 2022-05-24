#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup.sh

testParseTreeEqual2() {
    checkParseTreeEqual mult3.exp "[MULT]([MULT](2, 3), 4)"
}

testParseTreeEqual3() {
    checkParseTreeEqual assign.exp "[EQUAL](x, [EQUAL](y, [EQUAL](z, 16)))"
}

testParseTreeEqual4() {
    checkParseTreeEqual multsin.exp "[MULT]([MULT]([sin](., 2), [sin](., 3)), [sin](., 4))"
}


testParseTreeEqual_r_1() {
    checkParseTreeEqual right.exp "[r](1, [r](2, [s](3, [s](4, 5))))"
}
testParseTreeEqual_r_2() {
    checkParseTreeEqual right2.exp "[r]([s](1, [s](2, 3)), [r](4, 5))"
}


testParseTreeEqual_unary_binary11() {
    checkParseTreeEqual unary-binary11.exp "[PLUS]([PLUS](., 1), 2)"
}

testParseTreeEqual_unary_binary12() {
    checkParseTreeEqual unary-binary12.exp "[MINUS]([MINUS](., 1), 2)"
}

testParseTreeEqual_question_mark() {
    # see what happens when unary follows binary with higher prec
    checkParseTreeEqual question.exp "[EQUAL]([JUXTA](Is, [MULT]([MULT](2, 2), 2)), [QUESTION](7))"
}

testParseTreeEqual_unary_binary_valid2() {
    checkParseTreeEqual ub2.exp "[ub]([ub](1, 2), 3)"
}

testParseTreeEqual_binary_invalid() {
    checkParseTreeEqual plus2.exp "[PLUS]([PLUS](1), 2)"
}

testParseTreeEqual_binary_invalid2() {
    checkParseTreeEqual eq2.exp "[EQUAL](1, [EQUAL](., 2))"
}

. ./myshunit2

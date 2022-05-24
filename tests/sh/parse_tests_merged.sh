#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup.sh

testParseTreeEqual_unary_binary_valid() {
    checkParseTreeEqual ub2.exp "[ub]([ub](1, 2), 3)"
    checkParseTreeEqual ub2.exp "[ub](1, 2, 3)" "-m"
}

testParseTreeEqual_binary_invalid1() {
    checkParseTreeEqual plus2.exp "[PLUS]([PLUS](1), 2)"
    checkParseTreeEqual plus2.exp "[PLUS](1, ., 2)" "-m"
}

testParseTreeEqual_binary_invalid2() {
    checkParseTreeEqual plus4.exp "[PLUS]([PLUS]([PLUS]([PLUS](1))), 2)"
    checkParseTreeEqual plus4.exp "[PLUS](1, ., ., ., 2)" "-m"
}

testParseTreeEqual_binary_invalid3() {
    checkParseTreeEqual eq2.exp "[EQUAL](1, [EQUAL](., 2))"
    checkParseTreeEqual eq2.exp "[EQUAL](1, ., 2)" "-m"
}

testParseTreeEqual_binary_invalid4() {
    checkParseTreeEqual eq4.exp "[EQUAL](1, [EQUAL](., [EQUAL](., [EQUAL](., 2))))"
    checkParseTreeEqual eq4.exp "[EQUAL](1, ., ., ., 2)" "-m"
}

testParseTreeEqual_binary_invalid4() {
    checkParseTreeEqual eq4.exp "[EQUAL](1, [EQUAL](., [EQUAL](., [EQUAL](., 2))))"
    checkParseTreeEqual eq4.exp "[EQUAL](1, ., ., ., 2)" "-m"
}

testParseTreeEqual_double_prefix() {
    checkParseTreeEqual uu.exp "[q](., [q](., A))"
    checkParseTreeEqual uu.exp "[q](., [q](., A))" "-m"
}

testParseTreeEqual_double_par_prefix() {
    checkParseTreeEqual uparupar.exp "[uopen](., [uclose](1, [uopen](., [uclose](2, A))))"
    checkParseTreeEqual uparupar.exp "[uopen](., [uclose](1, [uopen](., [uclose](2, A))))" "-m"
}

testParseTreeEqual_double_ub_prefix() {
    checkParseTreeEqual ubub.exp "[ub](., [ub](., A))"
    checkParseTreeEqual ubub.exp "[ub](., [ub](., A))" "-m"
}

testParseTreeEqual_double_ub_par_prefix() {
    checkParseTreeEqual ubparubpar.exp "[ubopen](., [ubclose](1, [ubopen](., [ubclose](2, A))))"
    checkParseTreeEqual ubparubpar.exp "[ubopen](., [ubclose](1, [ubopen](., [ubclose](2, A))))" "-m"
}

testParseTreeEqual_double_postfix() {
    checkParseTreeEqual pp.exp "[p]([p](A))"
    checkParseTreeEqual pp.exp "[p]([p](A))" "-m"
}

testParseTreeEqual_double_par_postfix() {
    checkParseTreeEqual pparppar.exp "[popen]([popen](A, [pclose](1)), [pclose](2))"
    checkParseTreeEqual pparppar.exp "[popen]([popen](A, [pclose](1)), [pclose](2))" "-m"
}

testParseTreeEqual_ub_prefix_binary() {
    checkParseTreeEqual ubub2.exp "[ub](A, [ub](., B))"
    checkParseTreeEqual ubub2.exp "[ub](A, [ub](., B))" "-m"
}

testParseTreeEqual_ub_par_prefix_binary() {
    checkParseTreeEqual ubparubpar2.exp "[ubopen](A, [ubclose](1, [ubopen](., [ubclose](2, B))))"
    checkParseTreeEqual ubparubpar2.exp "[ubopen](A, [ubclose](1, [ubopen](., [ubclose](2, B))))" "-m"
}

. ./myshunit2

#! /bin/zsh



export SHUNIT_PARENT=$0
. ./setup.sh

testParseTreeEqual_double_binary() {
    checkParseTreeEqual bin2.exp "[MULT]([MULT](A, B), C)"
    checkParseTreeEqual bin2.exp "[MULT](A, B, C)" "-m"
}

testParseTreeEqual_double_binary_right() {
    checkParseTreeEqual eq3.exp "[EQUAL](1, [EQUAL](2, 3))"
    checkParseTreeEqual eq3.exp "[EQUAL](1, 2, 3)" "-m"
}

testParseTreeEqual_double_par_binary() {
    checkParseTreeEqual bparbpar.exp "[bopen]([bopen](A, [bclose](1, B)), [bclose](2, C))"
    checkParseTreeEqual bparbpar.exp "[bopen](A, [bclose](1, B), [bclose](2, C))" "-m"
}

testParseTreeEqual_double_par_binary_right() {
    checkParseTreeEqual brparbrpar.exp "[brclose]([bropen](A, 1), [brclose]([bropen](B, 2), C))"
    checkParseTreeEqual brparbrpar.exp "[brclose]([bropen](A, 1), [bropen](B, 2), C)" "-m"
}

. ./myshunit2

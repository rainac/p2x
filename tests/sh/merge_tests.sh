#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup_sh.sh

checkParseTreeEqualLocal() {
    checkParseTreeEqual "$1" "$2" "--merged $3"
}

test_ParseTreeEqual2() {
    checkParseTreeEqualLocal mult3.exp "[MULT](2, 3, 4)"
}

test_ParseTreeEqual_r_1() {
    checkParseTreeEqualLocal right.exp "[r](1, 2, [s](3, 4, 5))"
}
test_ParseTreeEqual_r_2() {
    checkParseTreeEqualLocal right2.exp "[r]([s](1, 2, 3), 4, 5)"
}


. ./myshunit2

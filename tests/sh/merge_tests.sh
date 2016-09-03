#! /bin/zsh

. ./setup_sh.sh

checkParseTreeEqualLocal() {
    checkParseTreeEqual "$1" "$2" "--merged $3"
}

testParseTreeEqual2() {
    checkParseTreeEqualLocal mult3.exp "[MULT](2, 3, 4)"
}

testParseTreeEqual_r_1() {
    checkParseTreeEqualLocal right.exp "[r](1, 2, [s](3, 4, 5))"
}
testParseTreeEqual_r_2() {
    checkParseTreeEqualLocal right2.exp "[r]([s](1, 2, 3), 4, 5)"
}


. ./myshunit2

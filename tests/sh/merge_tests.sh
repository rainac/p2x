#! /bin/bash

#set -x

checkParseTreeEqual() {
    infile=$1
    exp=$2
    opts="--merged $3"
    p2x $opts -p ../../examples/configs/default ../../examples/in/$infile > res.xml
    xsltproc -o res.txt ../../src/xsl/parens.xsl res.xml
    res=$(cat res.txt)
    echo "$infile: $(cat ../../examples/in/$infile)  =>  $res"
    assertEquals "Parse tree is not in expected form: '$exp' != '$res'" "$exp" "$res"
    rm res.txt res.xml
}

testParseTreeEqual2() {
    checkParseTreeEqual mult3.exp "[MULT](2, 3, 4)"
}

testParseTreeEqual_r_1() {
    checkParseTreeEqual right.exp "[r](1, 2, [s](3, 4, 5))"
}
testParseTreeEqual_r_2() {
    checkParseTreeEqual right2.exp "[r]([s](1, 2, 3), 4, 5)"
}


. shunit2

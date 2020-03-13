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

testParseTreeEqual_null_in_merged1() {
    checkParseTreeEqual miss-ops1.exp '[NEWLINE]([COMMA](., ., x, ., .))' '' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged2() {
    checkParseTreeEqual miss-ops2.exp '[NEWLINE]([FULL_STOP]([FULL_STOP]([FULL_STOP]([FULL_STOP](), x))))' '' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged3() {
    checkParseTreeEqual miss-ops3.exp '[NEWLINE]([SEMICOLON](., ., x, ., .))' '' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged4() {
    checkParseTreeEqual miss-ops4.exp '[NEWLINE]([COLON](., [COLON](., [COLON](x, [COLON]()))))' '' '../../examples/configs/merged-binary'
}

teestParseTreeEqual_null_in_merged1_X() {
    checkParseTreeEqual miss-ops1.exp '' '-X' '../../examples/configs/merged-binary'
}
teestParseTreeEqual_null_in_merged2_X() {
    checkParseTreeEqual miss-ops2.exp '[NEWLINE]([FULL_STOP]([FULL_STOP]([FULL_STOP]([FULL_STOP](), x))))' '-X' '../../examples/configs/merged-binary'
}
teestParseTreeEqual_null_in_merged3_X() {
    checkParseTreeEqual miss-ops3.exp '' '-X' '../../examples/configs/merged-binary'
}
teestParseTreeEqual_null_in_merged4_X() {
    checkParseTreeEqual miss-ops4.exp '[NEWLINE]([COLON](., [COLON](., [COLON](x, [COLON]()))))' '-X' '../../examples/configs/merged-binary'
}

. ./myshunit2

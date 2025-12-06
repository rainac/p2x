#! /bin/zsh

export SHUNIT_PARENT=$0

. ./setup.sh

testP2X1() {
    $P2X notthere.txt > $tmpdir/log 2> $tmpdir/err
    assertNotEquals "P2X should fail in this case" $? 0
    err=$(cat $tmpdir/err)
    grep notthere.txt $tmpdir/err > /dev/null
    assertEquals "P2X should print the missing file name" $? 0
    grep -i fail $tmpdir/err > /dev/null
    assertEquals "P2X should say that it failed" $? 0
    grep -i open $tmpdir/err > /dev/null
    assertEquals "P2X should say 'open'" $? 0
    grep -i file $tmpdir/err > /dev/null
    assertEquals "P2X should say 'file'" $? 0
    grep -i in $tmpdir/err > /dev/null
    assertEquals "P2X should say 'in'" $? 0
}

testP2X2() {
    $P2X -p notthere.txt ../../examples/in/mult3.exp > $tmpdir/log 2> $tmpdir/err
    assertNotEquals "P2X should fail in this case" $? 0
    grep notthere.txt $tmpdir/err > /dev/null
    assertEquals "P2X should print the missing file name" $? 0
    grep -i fail $tmpdir/err > /dev/null
    assertEquals "P2X should say that it failed" $? 0
    grep -i open $tmpdir/err > /dev/null
    assertEquals "P2X should say 'open'" $? 0
    grep -i file $tmpdir/err > /dev/null
    assertEquals "P2X should say 'file'" $? 0
    grep -i config $tmpdir/err > /dev/null
    assertEquals "P2X should say 'config'" $? 0
}

testP2X3() {
    echo "a b c" | $P2X > /dev/null
    assertEquals "P2X should read from std input" $? 0
}

testP2X4() {
    echo "a b c" | $P2X | xsltproc ../../src/xsl/empty.xsl -  > /dev/null
    assertEquals "P2X should read from stdin and produce valid XML" $? 0
}

testP2X5() {
    echo -n "a b c" | $P2X $P2XFLAGS -i SPACE | xsltproc ../../src/xsl/parens.xsl - > $tmpdir/tmp.txt
    txt=$(cat $tmpdir/tmp.txt)
    assertEquals "P2X should read from stdin produce the correct XML" "$txt" "[JUXTA]([JUXTA](a, b), c)"
}

testP2X6() {
    if [[ -t 0 ]]; then
        $P2X -i ../../examples/in/mult3.exp
        assertNotEquals "P2X should fail in this case" $? 0
    fi
}

testP2X7() {
    $P2X $P2XFLAGS -S c --stdin-tty -T -p ../../examples/configs-special/cfuncs.p2c > $tmpdir/tmp.out
    assertEquals "P2X should not fail in this case" $? 0
    diff $tmpdir/tmp.out ../../examples/out/token-types.xml
    assertEquals "output should be the same as in fixture" "$?" 0
}

testP2X8() {
    $P2X $P2XFLAGS -o $tmpdir/res.xml -p ../../examples/configs/default ../../examples/in/noclose.exp 2> $tmpdir/err.txt
    xsltproc ../../src/xsl/parens.xsl $tmpdir/res.xml > $tmpdir/res.txt
    err=$(cat $tmpdir/err.txt)
    txt=$(cat $tmpdir/res.txt)
    grep "EOF" $tmpdir/res.xml > /dev/null
    assertEquals "P2X XML should not contain EOF token" "1" "$?"
    grep "Unexpected" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "end" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "R_PAREN" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    assertEquals "P2X should work and produce the correct XML" "[COMMA](int, [L_PAREN](., [NEWLINE]()))" "$txt"
}

testP2X9() {
    $P2X $P2XFLAGS -o $tmpdir/res.xml -p ../../examples/configs/default ../../examples/in/noclose2.exp 2> $tmpdir/err.txt
    xsltproc ../../src/xsl/parens.xsl $tmpdir/res.xml > $tmpdir/res.txt
    err=$(cat $tmpdir/err.txt)
    txt=$(cat $tmpdir/res.txt)
    #    cat res.xml
    grep "EOF" $tmpdir/res.xml > /dev/null
    assertEquals "P2X XML should not contain EOF token" "1" "$?"
    grep "Unexpected" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "'close'" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    assertEquals "P2X work and produce the correct XML" "[COMMA](int, [open](., [NEWLINE](3)))" "$txt"
}

testP2X10() {
    $P2X $P2XFLAGS -o $tmpdir/res.xml -p ../../examples/configs/default ../../examples/in/noclose3.exp 2> $tmpdir/err.txt
    xsltproc ../../src/xsl/parens.xsl $tmpdir/res.xml > $tmpdir/res.txt
    err=$(cat $tmpdir/err.txt)
    txt=$(cat $tmpdir/res.txt)
    grep "EOF" $tmpdir/res.xml > /dev/null
    assertEquals "P2X XML should not contain EOF token" "1" "$?"
    grep "Unexpected" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "'endblock'" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "'finish'" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "'endblock'" $tmpdir/err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    assertEquals "P2X work and produce the correct XML" "[COMMA](int, [open](., [begin](., 3)))" "$txt"
}

. ./myshunit2

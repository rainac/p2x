#! /bin/bash

#set -x

testP2X1() {
    p2x notthere.txt > log 2> err
    assertNotEquals "P2X should fail in this case" $? 0
    err=$(cat err)
    grep notthere.txt err > /dev/null
    assertEquals "P2X should print the missing file name" $? 0
    grep -i fail err > /dev/null
    assertEquals "P2X should say that it failed" $? 0
    grep -i open err > /dev/null
    assertEquals "P2X should say 'open'" $? 0
    grep -i file err > /dev/null
    assertEquals "P2X should say 'file'" $? 0
    grep -i in err > /dev/null
    assertEquals "P2X should say 'in'" $? 0
    rm err log
}

testP2X2() {
    p2x -p notthere.txt ../../examples/in/mult3.exp > log 2> err
    assertNotEquals "P2X should fail in this case" $? 0
    grep notthere.txt err > /dev/null
    assertEquals "P2X should print the missing file name" $? 0
    grep -i fail err > /dev/null
    assertEquals "P2X should say that it failed" $? 0
    grep -i open err > /dev/null
    assertEquals "P2X should say 'open'" $? 0
    grep -i file err > /dev/null
    assertEquals "P2X should say 'file'" $? 0
    grep -i config err > /dev/null
    assertEquals "P2X should say 'config'" $? 0
    rm err log
}

testP2X3() {
    echo "a b c" | p2x > /dev/null
    assertEquals "P2X should read from std input" $? 0
}

testP2X4() {
    echo "a b c" | p2x | xsltproc ../../src/xsl/empty.xsl -  > /dev/null
    assertEquals "P2X should read from stdin and produce valid XML" $? 0
}

testP2X5() {
    echo -n "a b c" | p2x -i SPACE | xsltproc ../../src/xsl/parens.xsl - > tmp.txt
    txt=$(cat tmp.txt)
    assertEquals "P2X should read from stdin produce the correct XML" "$txt" "[JUXTA]([JUXTA](a, b), c)"
    rm tmp.txt
}

testP2X6() {
    p2x -i ../../examples/in/mult3.exp
    assertNotEquals "P2X should fail in this case" $? 0
}

testP2X7() {
    p2x --stdin-tty -T -p ../../examples/configs/cfuncs > tmp.out
    assertEquals "P2X should not fail in this case" $? 0
    diff tmp.out ../../examples/out/token-types.xml
    assertEquals "$?" 0
    rm tmp.out
}

testP2X8() {
    p2x -o res.xml -p ../../examples/configs/default ../../examples/in/noclose.exp 2> err.txt
    xsltproc ../../src/xsl/parens.xsl res.xml > res.txt
    err=$(cat err.txt)
    txt=$(cat res.txt)
    grep "EOF" res.xml > /dev/null
    assertEquals "P2X XLM should not contain EOF token" "1" "$?"
    grep "unexpected" err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "end" err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "R_PAREN" err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    assertEquals "P2X should work and produce the correct XML" "[COMMA](int, [L_PAREN](., [NEWLINE]()))" "$txt"
    rm res.txt
}

testP2X9() {
    p2x -o res.xml -p ../../examples/configs/default ../../examples/in/noclose2.exp 2> err.txt
    xsltproc ../../src/xsl/parens.xsl res.xml > res.txt
    err=$(cat err.txt)
    txt=$(cat res.txt)
#    cat res.xml
    grep "EOF" res.xml > /dev/null
    assertEquals "P2X XLM should not contain EOF token" "1" "$?"
    grep "unexpected" err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    grep "\"close\"" err.txt > /dev/null
    assertEquals "P2X should print an error message" "0" "$?"
    assertEquals "P2X work and produce the correct XML" "[COMMA](int, [open](., [NEWLINE](3)))" "$txt"
    rm res.txt
}

. shunit2

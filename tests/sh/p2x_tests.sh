#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output

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

. shunit2

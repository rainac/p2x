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


# Test section indent
testP2X_indentation() {

    for kb in 1 2 3; do

        IN_KB=$kb P2XOPTS=--indent ./speed_test.sh > /dev/null 2> /dev/null
        ls -lrt /tmp/p2x-out.xml
        sz_noindent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)
        IN_KB=$kb P2XOPTS=--indent-unit=" " ./speed_test.sh > /dev/null 2> /dev/null
        ls -lrt /tmp/p2x-out.xml
        sz_indent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)
        IN_KB=$kb P2XOPTS=--indent-unit="  " ./speed_test.sh > /dev/null 2> /dev/null
        ls -lrt /tmp/p2x-out.xml
        sz_indent_sm2=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)

        let  incr=$(( (1.0*$sz_indent_sm)/$sz_noindent_sm ))
        let incr2=$(( (1.0*$sz_indent_sm2)/$sz_indent_sm  ))

        echo ""
        echo "*"
        echo "indenting increases file size by a factor of $incr"
        echo "indenting by twice the amount increases file size by a factor of $incr2"
        echo "*"
        echo ""

        test 1 -eq $(( $incr > 1 ))
        assertEquals "Indenting should increase file size" "0" "$?"

        test 1 -eq $(( $incr2 > 1 ))
        assertEquals "Indenting by twice the amount should increase file size by a factor approaching 2" "0" "$?"

        test 1 -eq $(( $incr2 < 2 ))
        assertEquals "Indenting by twice the amount should increase file size by a factor approaching 2" "0" "$?"

        rm /tmp/p2x-speed-in
        rm /tmp/p2x-out.xml
    done
}

testP2_indent_not_too_bad() {

    for kb in 1 10 20; do

        IN_KB=$kb P2XOPTS=--indent ./speed_test.sh > /dev/null 2> /dev/null
        ls -lrt /tmp/p2x-out.xml
        sz_noindent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)
        IN_KB=$kb ./speed_test.sh > /dev/null 2> /dev/null
        ls -lrt /tmp/p2x-out.xml
        sz_indent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)

        let incr=$sz_indent_sm/$sz_noindent_sm

        echo ""
        echo "*"
        echo "indenting increases file size by a factor of $incr"
        echo "*"
        echo ""

        test 1 -eq $(( $incr < 10 ))
        assertEquals "Indenting should not increase file size by more then 10 fold" "0" "$?"

        rm /tmp/p2x-speed-in
        rm /tmp/p2x-out.xml
    done
}

. shunit2

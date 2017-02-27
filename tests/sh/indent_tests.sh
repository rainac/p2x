#! /bin/zsh

# P2X shunit2 test suite: failing tests
#
# This suite runs some tests running p2x with different option
# controlling indentation of the XML output.

zmodload zsh/mathfunc

export SHUNIT_PARENT=$0
. ./setup_zsh.sh

# Test section indent
testP2X_indentation() {

    for kb in 1 2 3; do

        IN_KB=$kb P2XOPTS=--indent ./speed_test.sh > /dev/null 2> /dev/null
        assertEquals "P2X run must succeed" 0 $?
        ls -lrt /tmp/p2x-out.xml
        sz_noindent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)
        IN_KB=$kb P2XOPTS=--indent-unit=" " ./speed_test.sh > /dev/null 2> /dev/null
        assertEquals "P2X run must succeed" 0 $?
        ls -lrt /tmp/p2x-out.xml
        sz_indent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)
        IN_KB=$kb P2XOPTS=--indent-unit="  " ./speed_test.sh > /dev/null 2> /dev/null
        assertEquals "P2X run must succeed" 0 $?
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
        assertEquals "P2X run must succeed" 0 $?
        ls -lrt /tmp/p2x-out.xml
        sz_noindent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)
        IN_KB=$kb ./speed_test.sh > /dev/null 2> /dev/null
        assertEquals "P2X run must succeed" 0 $?
        ls -lrt /tmp/p2x-out.xml
        sz_indent_sm=$(ls -l /tmp/p2x-out.xml | cut -d ' ' -f 5)

        let incr=$(( 1.0*$sz_indent_sm/$sz_noindent_sm ))

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

testP2X_indentation_logoff() {

    for kb in 1 2 3; do

    done

}
        
. shunit2

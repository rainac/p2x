#! /bin/bash

mydir=$(dirname $BASH_SOURCE)

runvalgrind() {
    valgrind --error-exitcode=101 --leak-check=full --show-leak-kinds=all p2x $P2XOPTS -p $mydir/../../examples/configs/default $INF > /dev/null
    res=$?
    echo "valgrind exit code: $res"
    assertNotEquals "Valgrind reports errors" 101 $res
    ecCrash=$(( $res > 128))
    assertEquals "Program crashed" 0 $ecCrash
    rm -f out.txt
}


test_valgrind_mk_input() {
    cat $mydir/../../examples/in/*.exp > in.txt
    INF=in.txt
}

test_valgrind_no_errors() {
    P2XOPTS=" -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_noout() {
    P2XOPTS=""
    runvalgrind
}

test_valgrind_no_errors_X() {
    P2XOPTS="-X -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_M() {
    P2XOPTS="-M -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_J() {
    P2XOPTS="-J -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_innotexist() {
    INF="in2.txt"
    rm -f $INF
    P2XOPTS=" -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_noin() {
    INF=""
    P2XOPTS=" -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_scan() {
    P2XOPTS="-s -o out.txt"
    runvalgrind
}

test_valgrind_no_errors_scan_re2c() {
    P2XOPTS="-S re2c_c -s -o out.txt"
    runvalgrind
}

test_valgrind_cleanup() {
    rm -f in.txt
}

. shunit2

#! /bin/zsh

# P2X shunit2 test suite: failing tests
#
# This suite runs all the test scripts which fail currently and thus
# document missing features or bugs.


set -o shwordsplit
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output

run_suite() {
    echo "Run suite $1"
    ./$1
    assertEquals "TODO Test suite $1 unexpectedly has no errors or failures" 1 $?
}

test1() {
    run_suite false_test.sh
}

test1_invalid_conf_should_exit_nonz() {
    cat > fail.conf <<EOF
COMMA BINAY 200
EOF
    echo "ab c" | p2x -p fail.conf > log 2> err
    assertNotEquals "P2X should fail and report error here!" "0" "$?"
    rm log err
    rm fail.conf
}

testP2X_fail_fortran2() {
    p2x -p  ../../examples/configs/fort2.conf ../../examples/in/fortran2.exp > log 2> err
    assertEquals "P2X should not fail in this case" "0" "$?"
    grep -i "unexpected" err > /dev/null
    res=$?
    assertEquals "P2X should print and error message" "0" "$?"
    grep -i "end of input" err > /dev/null
    res=$?
    assertEquals "P2X should print and error message" "0" "$?"
    rm log err
}

. shunit2

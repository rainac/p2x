#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output

testP2X1() {
    p2x -p  ../../examples/configs/fort2.conf ../../examples/in/fortran1.exp > log 2> err
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

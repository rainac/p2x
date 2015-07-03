#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output

test_fails() {
    assertEquals "This cannot be true" 0 1
}
        
. shunit2

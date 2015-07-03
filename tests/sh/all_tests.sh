#! /bin/bash

run_suite() {
    echo "Run suite $1"
    ./$1
    assertEquals "Test suite $1 has errors or failures" 0 $?
}

test1() {
    run_suite val_tests.sh
}
test2() {
    run_suite reproduce_strict_tests.sh
}
test3() {
    run_suite reproduce_tests.sh
}
test4() {
    run_suite reparse_tests.sh
}
test5() {
    run_suite parse_tests.sh
}
test6() {
    run_suite merge_tests.sh
}
test7() {
    run_suite p2x_tests.sh
}

test_indentation() {
    run_suite indent_tests.sh
}

. shunit2

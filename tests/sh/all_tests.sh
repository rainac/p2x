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
    run_suite test_reproduce_strict.sh
}
test3() {
    run_suite test_reproduce.sh
}
test4() {
    run_suite test_reparse.sh
}
test5() {
    run_suite parse_tests.sh
}
test6() {
    run_suite merge_tests.sh
}
test_p2x_suite() {
    run_suite p2x_tests.sh
}
test8() {
    run_suite test_cmp_js.sh
}
test9() {
    run_suite p2xjs_mocha_tests.sh
}
test10() {
    run_suite test_output_stability.sh
}

test_indentation() {
    run_suite indent_tests.sh
}

test_inst() {
    run_suite install_tests.sh
}

test_performance() {
    run_suite perf_tests.sh
}

test_p2x_suite2() {
    run_suite p2x_tests2.sh
}

. shunit2

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
test_p2x_suite() {
    run_suite p2x_tests.sh
}

test_indentation() {
    run_suite indent_tests.sh
}

test_performance() {
    run_suite perf_tests.sh
}

test_p2x_suite2() {
    run_suite p2x_tests2.sh
}

test_output_modey() {
    run_suite output_modey.sh
}

test_output_mode_matlab() {
    run_suite output_mode_matlab.sh
}

test_output_mode_matlab_merged() {
    run_suite output_mode_matlab_merged.sh
}

test_output_mode_json() {
    run_suite output_mode_json.sh
}

test_output_mode_json_merged() {
    run_suite output_mode_json_merged.sh
}

test_large_scale() {
    run_suite large_scale.sh
}

test_valgrind() {
    run_suite valgrind_tests.sh
}

. shunit2

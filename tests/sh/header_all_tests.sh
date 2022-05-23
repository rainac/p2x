#! /bin/bash

run_suite() {
    echo "Run suite $1"
    ./$1 > log_$1.txt 2> err_$1.txt
    res=$?
    assertEquals "Test suite $1 has errors or failures" 0 $res
    if test "$res" = "0"; then
	echo $1 >> ok.txt
    else
	echo $1 >> fail.txt
    fi
}


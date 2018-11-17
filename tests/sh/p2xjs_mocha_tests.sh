#! /bin/bash

#set -x

testP2JS_Mocha() {
    (cd ../../src-js && mocha)
    assertEquals "P2XJS mocha tests should pass" $? 0
}

. shunit2

#! /bin/bash

#set -x

testP2JS_Mocha() {
    export P2X_HOME=/usr/local
    (cd ../../src-js && mocha)
    assertEquals "P2XJS mocha tests should pass" $? 0
}

. shunit2

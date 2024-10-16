#! /bin/bash

test_fails() {
    assertEquals "This cannot be true" 0 1
}
        
. shunit2

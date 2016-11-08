#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup_zsh.sh

testParseTreeEqual1() {
  env -i               ./parse_tests_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual2() {
  env -i  P2XFLAGS=-m  ./parse_tests_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual3() {
  env -i               ./parse_tests_not_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

. ./myshunit2

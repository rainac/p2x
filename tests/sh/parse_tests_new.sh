#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup_zsh.sh

testParseTreeEqual1() {
  env -u P2XFLAGS  ./parse_tests_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual2() {
  env -u P2XFLAGS  ./parse_tests_not_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual3() {
  env P2XFLAGS=-m  ./parse_tests_same_when_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

testParseTreeEqual4() {
  env -u P2XFLAGS  ./parse_tests_merged.sh
  assertEquals "sub test should pass" "0" "$?"
}

. ./myshunit2

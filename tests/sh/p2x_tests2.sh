#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output

testP2X1() {
    p2x -p  ../../examples/configs/fort2.conf ../../examples/in/fortran1.exp > log 2> err
    assertEquals "P2X should not fail in this case" "0" "$?"
    grep -i "unexpected" err > /dev/null
    res=$?
    assertEquals "P2X should print and error message" "0" "$?"
    grep -i "end of input" err > /dev/null
    res=$?
    assertEquals "P2X should print and error message" "0" "$?"
    rm log err
}

testP2X_fail_fortran2() {
    p2x -p  ../../examples/configs/fort2.conf ../../examples/in/fortran2.exp > log 2> err
    assertEquals "P2X should not fail in this case" "0" "$?"
    grep -i "unexpected" err > /dev/null
    res=$?
    assertEquals "P2X should print and error message" "0" "$?"
    grep -i "end of input" err > /dev/null
    res=$?
    assertEquals "P2X should print and error message" "0" "$?"
    rm log err
}

testP2X_no_output_newline() {
    p2x --indent -p  ../../examples/configs/default ../../examples/in/email.exp > res.xml 2> err
    assertEquals "P2X should not fail in this case" "0" "$?"
    numl=$(wc res.xml | awk '{print $1}')
    test $numl = 2
    assertEquals "P2X should print no newlines in XML by default" "0" "$?"
    rm err res.xml
}


testP2X_output_newline_if_desired() {
    p2x --indent --newline-as-br -p  ../../examples/configs/default ../../examples/in/email.exp > res.xml 2> err
    assertEquals "P2X should not fail in this case" "0" "$?"
    numl=$(wc res.xml | awk '{print $1}')
    test $numl = 6
    assertEquals "P2X should print newlines in XML if desired" "0" "$?"
    rm err res.xml
}

testP2X_output_newline_as_entity_if_desired() {
    p2x --indent --newline-as-entity -p  ../../examples/configs/default ../../examples/in/email.exp > res.xml 2> err
    assertEquals "P2X should not fail in this case" "0" "$?"
    numl=$(wc res.xml | awk '{print $1}')
    test $numl = 2
    assertEquals "P2X should print no newlines in XML when desired" "0" "$?"
    grep -E "&#xa;" res.xml > /dev/null
    assertEquals "P2X should print newlines as character entities if desired" "0" "$?"
    rm err res.xml
}

. shunit2

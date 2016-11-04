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

check_xml_output_no_added_whitespace_if_indent_off() {
    p2x $P2XFLAGS --indent -p  ../../examples/configs/default -o res.xml ../../examples/in/csv.exp
    assertEquals "P2X should not fail in this case" "0" "$?"
    xsltproc ../../src/xsl/empty.xsl res.xml > res.txt
    tr '[:space:]' 'X' < res.txt | grep 'X' > /dev/null
    assertEquals "P2X should not print any added whitespace if indent is off" "1" "$?"
    rm res.xml res.txt
}

check_output_no_added_whitespace_if_indent_off() {
    p2x $P2XFLAGS --indent -p  ../../examples/configs/default -o res.xml ../../examples/in/csv.exp
    assertEquals "P2X should not fail in this case" "0" "$?"
    tr '[:space:]' 'X' < res.xml | grep 'X' > /dev/null
    assertEquals "P2X should not print any added whitespace if indent is off" "1" "$?"
    rm res.xml
}

testP2X_output_no_added_whitespace_if_indent_off() {
    P2XFLAGS=""
    check_xml_output_no_added_whitespace_if_indent_off
}

testP2X_output_no_added_whitespace_if_indent_off_X() {
    P2XFLAGS="-X"
    check_xml_output_no_added_whitespace_if_indent_off
}

testP2X_output_no_added_whitespace_if_indent_off_M() {
    P2XFLAGS="-M"
    check_output_no_added_whitespace_if_indent_off
}

testP2X_output_no_added_whitespace_if_indent_off_J() {
    P2XFLAGS="-J"
    check_output_no_added_whitespace_if_indent_off
}

testP2X_error_in_config_reported() {
    for fc in ../../examples/configs-fail/*; do
        p2x $P2XFLAGS --indent -p $fc -o res.xml ../../examples/in/csv.exp
        assertNotEquals "P2X should exit with error" "0" "$?"
    done
}

testP2X_error_when_config_is_dir() {
    p2x $P2XFLAGS --indent -p ../../examples/configs -o res.xml ../../examples/in/csv.exp
    assertNotEquals "P2X should exit with error" "0" "$?"
}

. shunit2

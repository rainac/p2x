#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output
TMP=${TMP:-/tmp}

test_LargeCVS1_create() {
    yes Abc,123,456.789 | head -n 1000000 > $TMP/large.csv
    sz1=$(ls -l $TMP/large.csv | cut -d " " -f 5)
}

test_LargeCVS1_parse2XML() {
    time p2x --indent -m -bNEWLINE -bCOMMA -iSPACE -o res.xml $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.xml | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"
}

test_LargeCVS1_parse2XML_Y() {
    time p2x --indent -m --output-mode=y -bNEWLINE -bCOMMA -iSPACE -o res.xml $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.xml | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"
}

test_LargeCVS1_parse2MATLAB() {
    time p2x --indent -m -M -bNEWLINE -bCOMMA -iSPACE -o res.m $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.m | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"
}

test_LargeCVS1_parse2JSON() {
    time p2x --indent -m -J -bNEWLINE -bCOMMA -iSPACE -o res.json $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.json | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"
}

test_LargeCVS1_cleanup() {
    rm -f $TMP/large.csv res.json res.m res.xml
}

. shunit2

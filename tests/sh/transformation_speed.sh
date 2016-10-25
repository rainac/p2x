#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output
TMP=${TMP:-/tmp}

P2XOPTS=(-Vtimes,io)

test_LargeCVS1_create() {
    yes Abc,123,456.789 | head -n 1000000 > $TMP/large.csv
    sz1=$(ls -l $TMP/large.csv | cut -d " " -f 5)
}

test_LargeCVS1_parse2XML_addcols_xslt() {
    time p2x $P2XOPTS --indent -m -bNEWLINE -bCOMMA -iSPACE -o res.xml $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.xml | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"

    time xsltproc -o res.dat add-columns23.xsl res.xml
}

test_LargeCVS1_parse2XML_Y_addcols_xslt() {
    time p2x $P2XOPTS --indent -m --output-mode=y -bNEWLINE -bCOMMA -iSPACE -o res.xml $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.xml | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"

    time xsltproc -o res.dat add-columns23.xsl res.xml
}

test_LargeCVS1_parse2JSON_addcols_py() {
    time p2x $P2XOPTS --indent -m -J -bNEWLINE -bCOMMA -iSPACE -o res.json $TMP/large.csv
    assertEquals "P2X should not fail in this case" 0 $?
    sz2=$(ls -l res.json | cut -d " " -f 5)
    ratio=$(( 1.0 * $sz2 / $sz1 ))
    echo "Output size ratio: $ratio"

    time (python add-columns23.py res.json > res.dat)
}


test_LargeCVS1_cleanup() {
    rm -f $TMP/large.csv res.json res.m res.xml
}

. shunit2

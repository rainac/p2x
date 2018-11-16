#! /bin/zsh

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output
TMP=${TMP:-/tmp}

checkScannerEquiv() {
    sc1=$1
    sc2=${2:-re2c_$1}
    p2x -s --attribute-id -S $sc1 -o res1.xml input.txt
    p2x -s --attribute-id -S $sc2 -o res2.xml input.txt
    sed -i -e '/ca:scanner/ d' res1.xml
    sed -i -e '/ca:scanner/ d' res2.xml
    diff res1.xml res2.xml
    res=$?
    assertEquals "P2X produce identical results in with scanners $sc1 vs. $sc2" 0 $res
}

test_scannerTests_createInput() {
    ./genrandtoken.py 100000 > input.txt
    sz1=$(ls -l input.txt | cut -d " " -f 5)
}

test_scannerTestC() {
    checkScannerEquiv C
}

test_scannerTestR() {
    checkScannerEquiv R
}

test_scannerTestM() {
    checkScannerEquiv M
}

test_scannerTests_cleanup() {
    rm -f res1.xml res2.xml input.txt
}

. shunit2

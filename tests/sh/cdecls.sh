#! /bin/zsh

export SHUNIT_PARENT=$0

. ./setup.sh

setUp() {
    :
}

tearDown() {
    :
}

parseCCode() {
    inf=$1
    $P2X -gmXp $exdir/configs/cdecls $inf | tee cdecls.xml | xsltproc $exdir/xsl/cdecls.xsl -
    ps=($pipestatus)
    assertEquals "${ps[1]}" "0"
    assertEquals "${ps[2]}" "0"
    assertEquals "${ps[3]}" "0"
}

test_cdecls1() {
    parseCCode $exdir/in/cdecls.exp
}

. ./myshunit2

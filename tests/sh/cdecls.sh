#! /bin/bash

. ./setup.sh

setUp() {
    :
}

tearDown() {
    rm cdecls.xml
}

parseCCode() {
    inf=$1
    $P2X -gmXp $exdir/configs/cdecls $inf | tee cdecls.xml | xsltproc $exdir/xsl/cdecls.xsl -
    ps=("${PIPESTATUS[@]}")
    assertEquals "${ps[0]}" "0"
    assertEquals "${ps[1]}" "0"
    assertEquals "${ps[2]}" "0"
}

test_cdecls1() {
    parseCCode $exdir/in/cdecls.exp
}

. ./myshunit2

#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup_zsh.sh

parseCCode() {
    inf=$1
#    p2x -gmp ../../examples/configs/cdecls $inf | xsltproc ../../src/xsl/parens.xsl -
    p2x -gmXp ../../examples/configs/cdecls $inf | xsltproc ../../examples/xsl/cdecls.xsl -
}

test_cdecls1() {
    parseCCode ../../examples/in/cdecls.exp
}

. ./myshunit2

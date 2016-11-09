#! /bin/bash

. setup_sh.sh

checkExpFile() {
    i=$1
    echo "Parse file $i"
    opts=""
    reprxsl=reproduce.xsl
    if test "$i" = "german.exp" \
           || test "$i" = "letter.exp" \
           || test "$i" = "fliesst.exp"
    then
        opts="-e latin1"
        reprxsl=reproduce-latin1.xsl
    fi
    p2x $P2XFLAGS $opts -p ../../examples/configs/default ../../examples/in/$i > $tmpdir/res.xml
    xsltproc ../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
    diff ../../examples/in/$i $tmpdir/res.txt > /dev/null
    assertEquals "Reproduce test $i did not return same result" 0 $?
}

testReproduce() {
    checkExpFile bin-paren-empty.exp
}

#. shunit2

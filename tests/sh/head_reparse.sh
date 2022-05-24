#! /bin/bash

. setup.sh

checkExpFile() {
    i=$1
    echo "Parse file $i"
    opts="--indent --newline-as-br"
    reprxsl=empty.xsl
    if [[ "$i" = "german.exp" ]] \
           || [[ "$i" = "letter.exp" ]] \
           || [[ "$i" = "fliesst.exp" ]]
    then
        reprxsl=empty-latin1.xsl
        opts="$opts -e latin1"
    fi
    if [[ "$i" = "cr.exp" ]]
    then
        reprxsl=reproduce.xsl
        opts=""
    fi
    p2x $PSXFLAGS $opts -p ../../examples/configs/default ../../examples/in/$i > $tmpdir/res.xml
    xsltproc ../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
    p2x $PSXFLAGS $opts -p ../../examples/configs/default $tmpdir/res.txt > $tmpdir/res2.xml
    diff $tmpdir/res.xml $tmpdir/res2.xml > /dev/null
    assertEquals "Reparse test $i did not return same result" 0 $?
}


testParseTwiceIdentical() {
    checkExpFile email.exp
}


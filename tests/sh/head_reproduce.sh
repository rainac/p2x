#! /bin/bash

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
    p2x $opts -p ../../examples/configs/default ../../examples/in/$i > res.xml
    xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
    diff ../../examples/in/$i res.txt > /dev/null
    assertEquals "Reproduce test $i did not return same result" 0 $?
    rm res.xml res.txt
}

testReproduce() {
    checkExpFile bin-paren-empty.exp
}

#. shunit2

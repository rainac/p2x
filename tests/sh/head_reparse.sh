#! /bin/bash

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
    p2x $opts -p ../../examples/configs/default ../../examples/in/$i > res.xml
    xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
    p2x $opts -p ../../examples/configs/default res.txt > res2.xml
    diff res.xml res2.xml > /dev/null
    assertEquals "Reparse test $i did not return same result" 0 $?
    rm res.xml res2.xml res.txt
}


testParseTwiceIdentical() {
    checkExpFile email.exp
}


#! /bin/bash

mydir=$(dirname $BASH_SOURCE)

. $mydir/setup_tmp.sh

checkExpFile() {
    i=$1
    echo "Parse file $i"
    opts=""
    reprxsl=reproduce.xsl
    p2xjs $opts -S $mydir/../../examples/configs/scanner-simple.json -p $tmpdir/conf-with-ignores $mydir/../../examples/in/$i > $tmpdir/res.xml
    xsltproc $mydir/../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
    diff $mydir/../../examples/in/$i $tmpdir/res.txt > /dev/null
    assertEquals "Reproduce test $i did not return same result" 0 $?
}

testGenConfig() {
    cp $mydir/../../examples/configs/default $tmpdir/conf-with-ignores
    echo "\"ign\" ignore 1" >> $tmpdir/conf-with-ignores
    echo "\"IGN\" ignore 1" >> $tmpdir/conf-with-ignores
}

testReproduce1() {
    checkExpFile bin-paren-empty.exp
}

testReproduce2() {
    checkExpFile text-with-ignores.exp
}

testReproduce3() {
    checkExpFile text-with-unknown-chars.exp
}

testReproduce4() {
    checkExpFile word-with-unknown-chars.exp
}

testReproduce5() {
    checkExpFile gefaehrlich.exp
}

. $mydir/myshunit2

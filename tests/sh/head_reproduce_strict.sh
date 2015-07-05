#! /bin/bash

tmpdir=${TMP:-/tmp}/shnuit2-test-$$
mkdir $tmpdir

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
    p2x $opts -p ../../examples/configs/default ../../examples/in/$i > $tmpdir/res.xml
    xsltproc ../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
    diff ../../examples/in/$i $tmpdir/res.txt > /dev/null
    assertEquals "Reproduce test $i did not return same result" 0 $?
}

testReproduceStrict() {
    checkExpFile email.exp
}

#! /bin/bash

. setup.sh

checkExpFile() {
    i=$1
    opts=""
    stored_out=../../examples/out/$(basename $i .exp).xml
    if test "$i" = "letter.exp" || test "$i" = "german.exp" || test "$i" = "fliesst.exp"; then
        opts="-e latin1"
    fi
    if test -f "$stored_out"; then
        echo "Parse file $i"

        $P2X $P2XFLAGS $opts -p ../../examples/configs/default -o $tmpdir/res.xml ../../examples/in/$i
        assertEquals "P2X command must succeed" 0 $?

        sed -e '/<ca:version/ d' $tmpdir/res.xml > $tmpdir/res2.xml
        xsltproc -o $tmpdir/res3.xml ../../src/xsl/copy.xsl $tmpdir/res2.xml
        assertEquals "xsltproc must succeed" 0 $?


        sed -e '/<ca:version/ d' $stored_out > $tmpdir/check.xml
        xsltproc -o $tmpdir/check2.xml ../../src/xsl/copy.xsl $tmpdir/check.xml
        assertEquals "xsltproc must succeed" 0 $?

        diff $tmpdir/check2.xml $tmpdir/res3.xml
        assertEquals "Output has changed to the stored version" 0 $?
    fi
}

testReproduceStrict() {
    checkExpFile email.exp
}

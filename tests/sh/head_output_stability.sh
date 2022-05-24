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

        rm -f $tmpdir/res.xml $tmpdir/res2.xml
        touch $tmpdir/res2.xml

        $P2X $P2XFLAGS $opts -p ../../examples/configs/default ../../examples/in/$i > $tmpdir/res.xml
        assertEquals "P2X command must succeed" 0 $?

        xsltproc -o $tmpdir/res2.xml ../../src/xsl/but-root.xsl $tmpdir/res.xml

        sed -e 's/ code="[^"]*"//' $tmpdir/res2.xml  > $tmpdir/res3.xml

        sed -e 's/ code="[^"]*"//' $stored_out | \
            sed -e 's/ line="[^"]*"//'  | \
            sed -e 's/ col="[^"]*"//'  > $tmpdir/check.xml

        diff $tmpdir/check.xml $tmpdir/res3.xml
        assertEquals "Output has changed to the stored version" 0 $?
    else
        :
        #echo "no stored output for $i: $stored_out"
    fi
}

testReproduceStrict() {
    checkExpFile email.exp
}

#! /bin/bash
#set -x
checkExpFile() {
    i=$1
    opts=""
    stored_out=../../examples/out/$(basename $i .exp).xml
    if test "$i" = "ftp2.exp" || test "$i" = "f1.exp" || test "$i" = "fl1rg.exp" || test "$i" = "1p2ep3.exp"; then
        return
    fi
    if test "$i" = "letter.exp" || test "$i" = "german.exp" || test "$i" = "fliesst.exp"; then
        opts="-e latin1"
    fi
    if test -f "$stored_out"; then
        echo "Parse file $i"

        rm -f res.xml res2.xml
        p2x $opts -p ../../examples/configs/default ../../examples/in/$i > res.xml
        xsltproc -o res2.xml ../../src/xsl/but-root.xsl res.xml

        rm -f check.xml
        sed -e 's/ code="[^"]*"//' $stored_out  > check.xml

        diff check.xml res2.xml
        assertEquals "Output has changed to the stored version" 0 $?
        rm res.xml check.xml res2.xml
    else
        :
        #echo "no stored output for $i: $stored_out"
    fi
}

testReproduceStrict() {
    checkExpFile email.exp
}

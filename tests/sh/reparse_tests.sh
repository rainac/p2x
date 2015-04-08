#! /bin/bash

testParseTwiceIdentical() {

    for i in ../../examples/in/*.exp; do
        echo "Parse file $i"
        opts="--indent --newline-as-br"
        reprxsl=empty.xsl
        if [[ "$i" = "../../examples/in/german.exp" ]] \
            || [[ "$i" = "../../examples/in/letter.exp" ]] \
            || [[ "$i" = "../../examples/in/fliesst.exp" ]]
        then
            reprxsl=empty-latin1.xsl
            opts="$opts -e latin1"
        fi
        p2x $opts -p ../../examples/configs/default $i > res.xml
        xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
        p2x $opts -p ../../examples/configs/default res.txt > res2.xml
        diff res.xml res2.xml > /dev/null
        assertEquals "Reparse test $i did not return same result" 0 $?
        rm res.xml res2.xml res.txt
    done

}

. shunit2

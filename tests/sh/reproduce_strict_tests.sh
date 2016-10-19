#! /bin/bash

testReproduceStrict() {

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
        if [[ "$i" = "../../examples/in/cr.exp" ]]
        then
            reprxsl=reproduce.xsl
            opts=""
        fi
        p2x $opts -p ../../examples/configs/default $i > res.xml
        xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
        diff $i res.txt > /dev/null
        assertEquals "Reproduce test $i did not return same result" 0 $?
        rm res.xml res.txt
    done

}

. shunit2

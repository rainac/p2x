#! /bin/bash

#set -x

testReproduce() {

    for i in ../../examples/in/*.exp; do
        echo "Parse file $i"
        opts=""
        reprxsl=reproduce.xsl
        if [[ "$i" = "../../examples/in/german.exp" ]] \
            || [[ "$i" = "../../examples/in/letter.exp" ]] \
            || [[ "$i" = "../../examples/in/fliesst.exp" ]]
        then
            opts="-e latin1"
            reprxsl=reproduce-latin1.xsl
        fi
        p2x $opts -p ../../examples/configs/default $i > res.xml
        xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
        diff $i res.txt > /dev/null
        assertEquals "Reproduce test $i did not return same result" 0 $?
        rm res.xml res.txt
    done

}

. shunit2

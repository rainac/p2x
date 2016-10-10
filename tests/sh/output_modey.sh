#! /bin/bash

#set -x

testReproduce() {

    for i in ../../examples/in/*.exp; do
        echo "Parse file $i"
        opts=""
        alt_opts="--output-mode=y"
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
        assertEquals "Plain reproduce test $i did not return same result" 0 $?

        sz1=$(ls -l res.xml | cut -d " " -f 5)

        p2x $opts $alt_opts -p ../../examples/configs/default $i > res.xml
        xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
        diff $i res.txt
        assertEquals "Alternate opts reproduce test $i did not return same result" 0 $?

        sz2=$(ls -l res.xml | cut -d " " -f 5)
        saving=$(( $sz1 / $sz2 ))
        echo "saved: $sz1 / $sz2 = $saving"

        rm res.xml res.txt

        test $saving -gt 0
        assertEquals "Alternate XML format should be smaller or of same size" 0 $?
    done

}

. shunit2

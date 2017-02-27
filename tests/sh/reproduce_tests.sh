#! /bin/bash

#set -x

testReproduce() {

  for c in ../../examples/configs/*; do
    echo "With config $c "
    if [[ "$c" = *.json ]]; then
        continue
    fi
    if [[ "$c" = *.xml ]]; then
        continue
    fi
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
        if [[ "$i" = "../../examples/in/cr.exp" ]]
        then
            reprxsl=reproduce.xsl
            opts=""
        fi
        p2x $opts -p $c $i > res.xml
        assertEquals "P2X exits without error" 0 $?
        xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
        assertEquals "xsltproc exits without error" 0 $?
        diff $i res.txt > /dev/null
        assertEquals "Reproduce test $i did not return same result" 0 $?
        rm res.xml res.txt
    done
  done

}

. shunit2

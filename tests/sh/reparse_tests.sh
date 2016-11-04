#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup_sh.sh

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
        if [[ "$i" = "../../examples/in/cr.exp" ]]
        then
            reprxsl=reproduce.xsl
            opts=""
        fi
        p2x $opts -p ../../examples/configs/default $i > $tmpdir/res.xml
        assertEquals "P2X should exit with status 0" 0 $?
        xsltproc ../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
        p2x $opts -p ../../examples/configs/default $tmpdir/res.txt > $tmpdir/res2.xml
        diff $tmpdir/res.xml $tmpdir/res2.xml > /dev/null
        assertEquals "Reparse test $i did not return same result" 0 $?
        rm $tmpdir/res.xml $tmpdir/res2.xml $tmpdir/res.txt
    done

}

. ./myshunit2

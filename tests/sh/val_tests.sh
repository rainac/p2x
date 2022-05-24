#! /bin/bash

. setup.sh

mydir=$(dirname $BASH_SOURCE)

grammar=$P2X_HOME/share/p2x/code-xml.rng
if ! [[ -f $grammar ]]; then
    grammar=$mydir/../../src/code-xml.rng
fi

testOutputValid() {
    for i in $mydir/../../examples/in/*.exp; do
        echo "Validate file $i"
        opts=""
        if [[ "$i" = "$mydir/../../examples/in/german.exp" ]] \
            || [[ "$i" = "$mydir/../../examples/in/letter.exp" ]] \
            || [[ "$i" = "$mydir/../../examples/in/fliesst.exp" ]]
        then
            opts="-e latin1"
        fi
        $P2X $P2XFLAGS $opts -p $mydir/../../examples/configs/default $i > $tmpdir/res.xml
#        runWithTimeout xmlstarlet val -e -r ../../src/code-xml.rng res.xml
        xmlstarlet val -b -e -r $grammar $tmpdir/res.xml
        assertEquals "Validating XML output for $i failed" 0 $?
#        rm $tmpdir/res.xml
    done

}

. myshunit2

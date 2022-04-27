#! /bin/bash

. setup_sh.sh

runWithTimeout() {
    rm -f done.txt fail.txt
    ($* && touch done.txt || touch fail.txt) &
    bpid=$!
#    echo "job started: $bpid"
    for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
        if [[ -e done.txt ]] || [[ -e fail.txt ]]; then
            break
        fi
        if ! [[ "$i" = 15 ]]; then
            sleep 0.1
        fi
    done
    if [[ -e done.txt ]]; then
        rm -f fail.txt
        rm done.txt
        true
    else
        if [[ -e fail.txt ]]; then
            rm fail.txt
            false
        else
            echo "The job $bpid did not terminate!"
            kill $bpid
            kill $bpid
            kill $bpid
            killall $1
            killall $1
            killall $1
            # its not my fault it didn't terminate
            true
        fi
    fi
}

mydir=$(dirname $BASH_SOURCE)

P2X_HOME=${P2X_HOME:-/usr}
P2X_SHARE_DIR=$P2X_HOME/share/p2x

GRAMMAR=$mydir/../../src/code-xml.rng
if ! [[ -f $GRAMMAR ]]; then
    GRAMMAR=$P2X_SHARE_DIR/code-xml.rng
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
        xmlstarlet val -b -e -r $GRAMMAR $tmpdir/res.xml
        assertEquals "Validating XML output for $i failed" 0 $?
#        rm $tmpdir/res.xml
    done

}

. myshunit2

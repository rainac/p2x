#! /bin/bash

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

testOutputValid() {

    for i in ../../examples/in/*.exp; do
        echo "Validate file $i"
        opts=""
        if [[ "$i" = "../../examples/in/german.exp" ]] \
            || [[ "$i" = "../../examples/in/letter.exp" ]] \
            || [[ "$i" = "../../examples/in/fliesst.exp" ]]
        then
            opts="-e latin1"
        fi
        p2x $opts -p ../../examples/configs/default $i > res.xml
#        runWithTimeout xmlstarlet val -e -r ../../src/code-xml.rng res.xml
        xmlstarlet val -b -e -r ../../src/code-xml.rng res.xml
        assertEquals "R test suite has errors or failures" 0 $?
        rm res.xml
    done

}

. shunit2


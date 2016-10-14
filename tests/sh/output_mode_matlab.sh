#! /bin/zsh

# This suite runs some tests running p2x with different option
# controlling the reproducibility of the XML output.

source common.sh
echo $0
export SHUNIT_PARENT=$0

alt_opts=""
common_opts=""

coproc octave --no-gui
octpid=$!
echo "computer" >&p
read -p FFF
echo "oct:$octpid>> $FFF"

test_Reproduce() {

    alt_opts_="$alt_opts"

    echo "1: Parse files with $common_opts"
    echo "2: Parse files with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
        ReproduceMatlab $i $alt_opts_ $common_opts
    done
    echo ""

}

test_ReproduceStrict() {

    alt_opts_="$alt_opts --strict"

    echo "1: Parse files with $common_opts"
    echo "2: Parse files with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
        ReproduceMatlab $i $alt_opts_ $common_opts
    done
    echo ""

}

test_QuitOctave() {

    echo "quit" >&p
    read -p FFF
    echo "oct:$octpid>> $FFF"

    wait $octpid
    octres=$?

    assertEquals "Octave should exit with status 0" 0 $octres
    
}

. shunit2

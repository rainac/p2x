#! /bin/zsh

# This suite runs some tests running p2x with different option
# controlling the reproducibility of the XML output.

source common.sh
echo $0
export SHUNIT_PARENT=$0

alt_opts="--output-mode=y"
common_opts=""

testReproduce() {

    alt_opts_="$alt_opts"

    echo "1: Parse file $i with $common_opts"
    echo "2: Parse file $i with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
        ReproduceTest $i $alt_opts_ $common_opts
    done
    echo ""

}

testReproduceIndented() {

    alt_opts_="$alt_opts --indent"

    echo "1: Parse file $i with $common_opts"
    echo "2: Parse file $i with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
#        echo "Parse test file $i"
        ReproduceTest $i $alt_opts_ $common_opts
    done
    echo ""

}

testReproduceMerged() {

    alt_opts_="$alt_opts --merged"

    echo "1: Parse file $i with $common_opts"
    echo "2: Parse file $i with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
        ReproduceTest $i $alt_opts_ $common_opts
    done
    echo ""

}

testReproduceIndentedMerged() {

    alt_opts_="$alt_opts --indent --merged"

    echo "1: Parse file $i with $common_opts"
    echo "2: Parse file $i with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
#        echo "Parse test file $i"
        ReproduceTest $i $alt_opts_ $common_opts
    done
    echo ""

}

. shunit2

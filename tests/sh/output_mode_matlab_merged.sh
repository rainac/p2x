#! /bin/zsh

# This suite runs some tests running p2x with different option
# controlling the reproducibility of the XML output.

source common.sh
echo $0
export SHUNIT_PARENT=$0

alt_opts=""
common_opts=""

test_ReproduceMerged() {

    alt_opts_="$alt_opts -m"

    echo "1: Parse files with $common_opts"
    echo "2: Parse files with $common_opts $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
	if test $(basename $i) = "low-prec-binary-seps.exp" \
	   || test $(basename $i) = "low-prec-binary-seps2.exp"
	then
	    # in merged mode with non-XML output it si not always possible to reproduce ignored token exactly
	    continue
	fi
        ReproduceMatlab $i "$alt_opts_" "$common_opts" "-w"
    done
    echo ""

}

test_ReproduceMerged_Indent() {

    common_opts_="$common_opts --indent"
    alt_opts_="$alt_opts -m"

    echo "1: Parse files with $common_opts_"
    echo "2: Parse files with $common_opts_ $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
	if test $(basename $i) = "low-prec-binary-seps.exp" \
	   || test $(basename $i) = "low-prec-binary-seps2.exp"
	then
	    # in merged mode with non-XML output it si not always possible to reproduce ignored token exactly
	    continue
	fi
        ReproduceMatlab $i $alt_opts_ "$common_opts_" "-w"
    done
    echo ""

}

test_QuitOctave() {
    ftest_QuitOctave
}

. shunit2

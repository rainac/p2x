#! /bin/zsh

# This suite runs some tests running p2x with different option
# controlling the reproducibility of the XML output.

source common.sh
echo $0
export SHUNIT_PARENT=$0

alt_opts=""
common_opts=""

test_ReproduceIndent() {

    common_opts_="$common_opts --merged"
    alt_opts_="$alt_opts --indent"

    echo "1: Parse files with $common_opts_"
    echo "2: Parse files with $common_opts_ $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
	if test $(basename $i) = "low-prec-binary-seps.exp" \
	   || test $(basename $i) = "low-prec-binary-seps2.exp" \
	   || test $(basename $i) = "line-comment2.exp"
	then
	    # in merged mode with non-XML output it si not always possible to reproduce ignored token exactly
	    continue
	fi
        ReproduceJSONPy $i $alt_opts_ $common_opts_ -w
    done
    echo ""

}

test_ReproduceStrict() {

    common_opts_="$common_opts --merged"
    alt_opts_="$alt_opts --strict"

    echo "1: Parse files with $common_opts_"
    echo "2: Parse files with $common_opts_ $alt_opts_"

    for i in ../../examples/in/*.exp; do
        #        echo "Parse test file $i"
	if test $(basename $i) = "low-prec-binary-seps.exp" \
	   || test $(basename $i) = "low-prec-binary-seps2.exp" \
	   || test $(basename $i) = "line-comment2.exp"
	then
	    # in merged mode with non-XML output it si not always possible to reproduce ignored token exactly
	    continue
	fi
        ReproduceJSONPy $i $alt_opts_ $common_opts_ -w
    done
    echo ""

}

test_QuitOctave() {
    ftest_QuitOctave
}

. ./myshunit2

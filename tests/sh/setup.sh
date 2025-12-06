
if [[ "$ZSH_NAME" == zsh ]]; then
    echo "is zsh"
    set -o shwordsplit
fi

export LANG=C # for grep used in shunit2 when running with zsh, depends on english output

export P2XFLAGS="$P2XFLAGS --output-mode=x"

top_srcdir=../..

P2X=${P2X:-$top_srcdir/src/p2x}
P2XJS=${P2XJS:-$top_srcdir/src-js/p2xjs}
exdir=$top_srcdir/examples

P2X_HOME=${P2X_HOME:-$(readlink -f $(dirname $(which $P2X))/..)}

TMP=${TMP:-/tmp}
tmpdir=${P2X_TMP:-${TMP}/shunit2-test-$$}
mkdir $tmpdir

function mycleanup() {
#    echo "CLEANUP"
    rm -rf $tmpdir
}

export SHUNIT_EXIT=mycleanup

checkParseTreeEqual() {
    infile=$1
    exp=$2
    opts="$3"
    config="${4:-../../examples/configs/default}"
    $P2X $P2XFLAGS $opts -p $config ../../examples/in/$infile > $tmpdir/res.xml
    xsltproc -o $tmpdir/res.txt ../../src/xsl/parens.xsl $tmpdir/res.xml
    res=$(cat $tmpdir/res.txt)
    echo "$infile: $(cat ../../examples/in/$infile)  =>  $res"
    assertEquals "Parse tree is not in expected form: '$exp' != '$res'" "$exp" "$res"
    rm $tmpdir/res.txt $tmpdir/res.xml
}


checkXforAllConfs() {
    cfac_cmd=$1
    for cfac_conf in  ../../examples/configs/*; do
        if [[ "$cfac_conf" = *.json ]]; then
            continue
        fi
        if [[ "$cfac_conf" = *.xml ]]; then
            continue
        fi
        export P2XCONF=$cfac_conf
#        echo "${cfac_cmd[*]}"
        ${cfac_cmd[*]}
    done
}

checkXforFlags() {
    cxff_cmd=$1
    cxff_flags=$2
    cxff_P2XFLAGS_=$P2XFLAGS
    for cxff_flag in $cxff_flags; do
#        echo "cxff_flag='$cxff_flag'"
        export P2XFLAGS="$cxff_P2XFLAGS_ $cxff_flag"
#        echo "P2XFLAGS: $P2XFLAGS"
#        echo "checkXforFlags: ${cxff_cmd[*]}"
        ${cxff_cmd[*]}
    done
    P2XFLAGS=$P2XFLAGS_
}

checkXforFlags2() {
    cxff_cmd=$1
    cxff_flags=$2
    cxff_flags2=$3
    cxff_P2XFLAGS_=$P2XFLAGS
    for cxff_flag in $cxff_flags; do
#        echo "cxff_flag='$cxff_flag'"
        for cxff_flag2 in $cxff_flags2; do
#            echo "cxff_flag2='$cxff_flag2'"
            export P2XFLAGS="$cxff_P2XFLAGS_ $cxff_flag $cxff_flag2"
#            echo "P2XFLAGS: $P2XFLAGS"
#            echo "checkXforFlags: ${cxff_cmd[*]}"
            ${cxff_cmd[*]}
        done
    done
    P2XFLAGS=$P2XFLAGS_
}

#! /bin/bash

#set -x

tmpdir=${TMP:-/tmp}/shnuit2-test-$$
mkdir $tmpdir

mkParseTree() {
    infile="$1"
    outfile="$2"
    p2x="$3"
    _opts="$4"
    xmltmp=$tmpdir/$(basename $outfile .txt).xml
    if test "$p2x" = "p2xjs"; then
        _opts="$_opts -S  ../../examples/configs/scanner-c.json"
    fi
    _opts="$_opts -o $xmltmp"
    cmd="$p2x $_opts -p ../../examples/configs/default ../../examples/in/$infile"
    echo "$cmd"
    $cmd
    xsltproc -o $outfile ../../src/xsl/parens.xsl $xmltmp
}

checkExpFile() {
    infile=$1
    opts=""

    skip=""
    case $infile in
        (utf8-ident.exp|german.utf8.exp|german.exp|fliesst.exp|letter.exp)
            skip=true
            echo "skip"
            ;;
    esac
    if test -n "$skip"; then
        return
    fi

    mkParseTree "$infile" "$tmpdir/res.txt" "p2x" "$opts -w"
    mkParseTree "$infile" "$tmpdir/res2.txt" "p2xjs" "$opts"
    
    xmlstarlet c14n $tmpdir/res.xml > $tmpdir/cres.xml
    xmlstarlet c14n $tmpdir/res2.xml > $tmpdir/cres2.xml

    diff $tmpdir/cres.xml $tmpdir/cres2.xml #> /dev/null
    assertEquals "XML parse trees C++/JS not equal" "$?" "0"
    diff $tmpdir/res.txt $tmpdir/res2.txt #> /dev/null
    assertEquals "Parse trees C++/JS not equal" "$?" "0"

}

testParseTreeEqual1() {
    checkExpFile bin-paren-empty.exp
}


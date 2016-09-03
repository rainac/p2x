
checkParseTreeEqual() {
    infile=$1
    exp=$2
    opts="$3"
    p2x $opts -p ../../examples/configs/default ../../examples/in/$infile > $tmpdir/res.xml
    xsltproc -o $tmpdir/res.txt ../../src/xsl/parens.xsl $tmpdir/res.xml
    res=$(cat $tmpdir/res.txt)
    echo "$infile: $(cat ../../examples/in/$infile)  =>  $res"
    assertEquals "Parse tree is not in expected form: '$exp' != '$res'" "$exp" "$res"
    rm $tmpdir/res.txt $tmpdir/res.xml
}


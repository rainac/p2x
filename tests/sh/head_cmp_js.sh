#! /bin/bash

#set -x

mkParseTree() {
    infile="$1"
    outfile="$2"
    p2x="$3"
    _opts="$4"
    xmltmp=$(basename $outfile .txt).xml
    if [[ "$p2x" = "p2xjs" ]]; then
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
    case $i in
        (utf8-ident.exp)
            skip=true
            ;;
    esac
    if [[ -n "$skip" ]]; then
        return
    fi

    mkParseTree "$infile" "res.txt" "p2x" "$opts"
    mkParseTree "$infile" "res2.txt" "p2xjs" "$opts"
    
    xmlstarlet c14n res.xml > cres.xml
    xmlstarlet c14n res2.xml > cres2.xml

    diff cres.xml cres2.xml #> /dev/null
    assertEquals "XML parse trees C++/JS not equal" "$?" "0"
    diff res.txt res2.txt #> /dev/null
    assertEquals "Parse trees C++/JS not equal" "$?" "0"

}

testParseTreeEqual1() {
    checkExpFile postfix-test.exp 
}


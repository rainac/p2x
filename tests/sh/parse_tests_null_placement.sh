#! /bin/zsh

export SHUNIT_PARENT=$0
. ./setup.sh

checkParseTreeEqual2() {
    infile=$1
    exp=$2
    opts="$3"
    config="${4:-../../examples/configs/default}"
    $P2X $opts -p $config ../../examples/in/$infile > $tmpdir/res.xml
    xmlstarlet el $tmpdir/res.xml > $tmpdir/res.txt
    res=$(cat $tmpdir/res.txt)
    echo "$infile: $(cat ../../examples/in/$infile)  => res.txt"
    if [[ "$exp" != "$res" ]]; then
        echo "**** soll ****"
        echo $exp
        echo "**** result ****"
        echo $res
        echo "****  ****"
    fi
    assertEquals "Parse tree is not in expected form" "$exp" "$res"
}

testParseTreeEqual_null_in_merged1() {
    checkParseTreeEqual miss-ops1.exp '[NEWLINE]([COMMA](., ., x, .))' '' '../../examples/configs/merged-binary'
    checkParseTreeEqual miss-ops1.exp '[NEWLINE]([COMMA](., ., x, ., .))' '--strict' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged2() {
    checkParseTreeEqual miss-ops2.exp '[NEWLINE]([FULL_STOP]([FULL_STOP]([FULL_STOP]([FULL_STOP](), x))))' '' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged3() {
    checkParseTreeEqual miss-ops3.exp '[NEWLINE]([SEMICOLON](., ., x, ., .))' '' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged4() {
    checkParseTreeEqual miss-ops4.exp '[NEWLINE]([COLON](., [COLON](., [COLON](x, [COLON]()))))' '' '../../examples/configs/merged-binary'
}

testParseTreeEqual_null_in_merged1_X() {
    read -r -d '' VAR <<'EOF'
code-xml
code-xml/c:version
code-xml/ROOT
code-xml/ROOT/null
code-xml/ROOT/NEWLINE
code-xml/ROOT/NEWLINE/COMMA
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/ID
code-xml/ROOT/NEWLINE/COMMA/ID/c:t
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/c:br
EOF
    checkParseTreeEqual2 miss-ops1.exp "$VAR" '-X' '../../examples/configs/merged-binary'
    read -r -d '' VAR <<'EOF'
code-xml
code-xml/c:version
code-xml/ROOT
code-xml/ROOT/null
code-xml/ROOT/NEWLINE
code-xml/ROOT/NEWLINE/COMMA
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/ID
code-xml/ROOT/NEWLINE/COMMA/ID/c:t
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/COMMA/c:t
code-xml/ROOT/NEWLINE/COMMA/null
code-xml/ROOT/NEWLINE/c:br
EOF
    checkParseTreeEqual2 miss-ops1.exp "$VAR" '-X --strict' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged2_X() {
    read -r -d '' VAR <<'EOF'
code-xml
code-xml/c:version
code-xml/ROOT
code-xml/ROOT/null
code-xml/ROOT/NEWLINE
code-xml/ROOT/NEWLINE/FULL_STOP
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/FULL_STOP
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/FULL_STOP/FULL_STOP
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/FULL_STOP/FULL_STOP/c:t
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/FULL_STOP/c:t
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/FULL_STOP/ID
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/FULL_STOP/ID/c:t
code-xml/ROOT/NEWLINE/FULL_STOP/FULL_STOP/c:t
code-xml/ROOT/NEWLINE/FULL_STOP/c:t
code-xml/ROOT/NEWLINE/c:br
EOF
    checkParseTreeEqual2 miss-ops2.exp "$VAR" '-X' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged3_X() {
    read -r -d '' VAR <<'EOF'
code-xml
code-xml/c:version
code-xml/ROOT
code-xml/ROOT/null
code-xml/ROOT/NEWLINE
code-xml/ROOT/NEWLINE/SEMICOLON
code-xml/ROOT/NEWLINE/SEMICOLON/null
code-xml/ROOT/NEWLINE/SEMICOLON/c:t
code-xml/ROOT/NEWLINE/SEMICOLON/null
code-xml/ROOT/NEWLINE/SEMICOLON/c:t
code-xml/ROOT/NEWLINE/SEMICOLON/ID
code-xml/ROOT/NEWLINE/SEMICOLON/ID/c:t
code-xml/ROOT/NEWLINE/SEMICOLON/c:t
code-xml/ROOT/NEWLINE/SEMICOLON/null
code-xml/ROOT/NEWLINE/SEMICOLON/c:t
code-xml/ROOT/NEWLINE/SEMICOLON/null
code-xml/ROOT/NEWLINE/c:br
EOF
    checkParseTreeEqual2 miss-ops3.exp "$VAR" '-X' '../../examples/configs/merged-binary'
}
testParseTreeEqual_null_in_merged4_X() {
    read -r -d '' VAR <<'EOF'
code-xml
code-xml/c:version
code-xml/ROOT
code-xml/ROOT/null
code-xml/ROOT/NEWLINE
code-xml/ROOT/NEWLINE/COLON
code-xml/ROOT/NEWLINE/COLON/null
code-xml/ROOT/NEWLINE/COLON/c:t
code-xml/ROOT/NEWLINE/COLON/COLON
code-xml/ROOT/NEWLINE/COLON/COLON/null
code-xml/ROOT/NEWLINE/COLON/COLON/c:t
code-xml/ROOT/NEWLINE/COLON/COLON/COLON
code-xml/ROOT/NEWLINE/COLON/COLON/COLON/ID
code-xml/ROOT/NEWLINE/COLON/COLON/COLON/ID/c:t
code-xml/ROOT/NEWLINE/COLON/COLON/COLON/c:t
code-xml/ROOT/NEWLINE/COLON/COLON/COLON/COLON
code-xml/ROOT/NEWLINE/COLON/COLON/COLON/COLON/c:t
code-xml/ROOT/NEWLINE/c:br
EOF
    checkParseTreeEqual2 miss-ops4.exp "$VAR" '-X' '../../examples/configs/merged-binary'
}

. ./myshunit2

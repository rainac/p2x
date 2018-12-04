#

zmodload zsh/mathfunc
set -o shwordsplit
export LANG=C # for grep used in shunit2, depends on english output

. ./setup_tmp.sh

trapSCHLD() {
    echo "SIGCHILD received"
}

trap trapSCHLD SIGCHLD

startOctave() {
    coproc octave --no-init-file -W --no-gui --path $PWD/../../src/m
    octpid=$!
    echo "computer" >&p
    read -p FFF
    echo "oct:$octpid>> $FFF"
    echo "datestr(now)" >&p
    read -p FFF
    echo "oct:$octpid>> $FFF"
}
startOctave

restartOctave() {
    echo "SIGCHILD received, restart Octave"
    startOctave
}

trap restartOctave SIGCHLD

ftest_QuitOctave() {

    trap "" SIGCHLD

    echo "quit" >&p
    read -p FFF
    echo "oct:$octpid>> $FFF"

    wait $octpid
    octres=$?

    assertEquals "Octave should exit with status 0" 0 $octres

}


ReproduceTest() {

    arg1_infile=$1
    arg1_alt_opts=${2:---output-mode=y}
    arg1_opts=${3:-""}
    eopts=""

    infile=$arg1_infile

    reprxsl=reproduce.xsl
    if [[ "$infile" = "../../examples/in/german.exp" ]] \
           || [[ "$infile" = "../../examples/in/letter.exp" ]] \
           || [[ "$infile" = "../../examples/in/fliesst.exp" ]]
    then
        eopts="-e latin1"
        reprxsl=reproduce-latin1.xsl
    fi

    opts="$eopts $arg1_opts"
    echo -n "Parse file $infile with '$opts', "
    p2x $opts -p ../../examples/configs/default $infile > $tmpdir/res.xml
    xsltproc ../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
    diff $infile $tmpdir/res.txt > /dev/null
    assertEquals "Plain reproduce test $infile did not return same result" 0 $?

    sz1=$(ls -l $tmpdir/res.xml | cut -d " " -f 5)

    opts=($eopts $arg1_opts $arg1_alt_opts)
    echo -n "Parse file $infile with '$opts'\r"
    p2x $opts -p ../../examples/configs/default $infile > $tmpdir/res.xml
    xsltproc ../../src/xsl/$reprxsl $tmpdir/res.xml > $tmpdir/res.txt
    diff $infile $tmpdir/res.txt
    assertEquals "Alternate opts reproduce test $infile did not return same result" 0 $?

    sz2=$(ls -l $tmpdir/res.xml | cut -d " " -f 5)
    saving=$(( 1.0 * $sz1 / $sz2 ))
#    echo "saved: $sz1 / $sz2 = $saving"

    [[ $saving -gt 1 ]]
#    assertEquals "Alternate XML format should be smaller or of same size: ($sz1 > $sz2)" 0 $?

}


ReproduceMatlab() {

    arg1_infile=$1
    arg1_alt_opts=$2
    arg1_opts=$3
    diffopts=$4
    eopts=""

    p2xopts="--output-mode=matlab"

    infile=$arg1_infile

    opts="$eopts $arg1_opts"
    echo -n "Parse with '$opts': file $infile"
    p2x $p2xopts $opts -p ../../examples/configs/default $infile > $tmpdir/res.m
    cat > $tmpdir/runscript.m <<EOF
clear all
run('$tmpdir/res.m');
fd = fopen('$tmpdir/res.txt', 'w');
fprintf(fd, '%s', reproduce(ans));
fclose(fd);
'done'
EOF
#    octave --eval runscript
    cat $tmpdir/runscript.m >&p
    read -p FFF
#    echo "oct>> $FFF"
    assertEquals "Output should be valid MATLAB code" 0 $?
    diff $diffopts $infile $tmpdir/res.txt
    assertEquals "Plain reproduce test $infile did not return same result" 0 $?

    sz1=$(ls -l $tmpdir/res.m | cut -d " " -f 5)

    opts=($eopts $arg1_opts $arg1_alt_opts)
    echo -n "Parse with '$opts': file $infile"
    p2x $p2xopts $opts -p ../../examples/configs/default $infile > $tmpdir/res2.m
    cat > $tmpdir/runscript.m <<EOF
clear all
run('$tmpdir/res2.m');
fd = fopen('$tmpdir/res.txt', 'w');
fprintf(fd, '%s', reproduce(ans));
fclose(fd);
'done'
EOF
#    octave --eval runscript
    cat $tmpdir/runscript.m >&p
    read -p FFF
#    echo "oct>> $FFF"
    assertEquals "Output should be valid MATLAB code" 0 $?
    diff $diffopts $infile $tmpdir/res.txt
    assertEquals "Alternate opts reproduce test $infile did not return same result" 0 $?
    echo -n "\r"

    sz2=$(ls -l $tmpdir/res2.m | cut -d " " -f 5)
    saving=$(( 1.0 * $sz1 / $sz2 ))
#    echo "saved: $sz1 / $sz2 = $saving"

    [[ $saving -ge 1 ]]
#    assertEquals "Alternate XML format should be smaller or of same size: ($sz1 > $sz2)" 0 $?

}


ReproduceJSONPy() {

    arg1_infile=$1
    arg1_alt_opts=$2
    arg1_opts=$3
    diffopts=$4
    eopts=""

    p2xopts="--output-mode=json"

    infile=$arg1_infile

    if [[ "$infile" = "../../examples/in/german.exp" ]] \
           || [[ "$infile" = "../../examples/in/letter.exp" ]] \
           || [[ "$infile" = "../../examples/in/fliesst.exp" ]]
    then
        return
    fi

    opts="$eopts $arg1_opts"
    echo -n "Parse with '$opts': file $infile"
    p2x $p2xopts $opts -p ../../examples/configs/default $infile > $tmpdir/res.json
    python $PWD/../../src/py/reproduce.py < $tmpdir/res.json > $tmpdir/res.txt
    assertEquals "Output should be valid MATLAB code" 0 $?
    diff $diffopts $infile $tmpdir/res.txt
    assertEquals "Plain reproduce test $infile did not return same result" 0 $?

    sz1=$(ls -l $tmpdir/res.json | cut -d " " -f 5)

    opts=($eopts $arg1_opts $arg1_alt_opts)
    echo -n "Parse with '$opts': file $infile"
    p2x $p2xopts $opts -p ../../examples/configs/default $infile > $tmpdir/res2.json
    python $PWD/../../src/py/reproduce.py < $tmpdir/res2.json > $tmpdir/res2.txt
    assertEquals "Output should be valid MATLAB code" 0 $?
    diff $diffopts $infile $tmpdir/res2.txt
    assertEquals "Alternate opts reproduce test $infile did not return same result" 0 $?
    echo -n "\r"

    sz2=$(ls -l $tmpdir/res2.json | cut -d " " -f 5)
    saving=$(( 1.0 * $sz1 / $sz2 ))
#    echo "saved: $sz1 / $sz2 = $saving"

    [[ $saving -ge 1 ]]
#    assertEquals "Alternate XML format should be smaller or of same size: ($sz1 > $sz2)" 0 $?

}

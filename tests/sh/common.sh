#

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
    echo -n "Parse file $infile with '$opts'\r"
    p2x $opts -p ../../examples/configs/default $infile > res.xml
    xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
    diff $infile res.txt > /dev/null
    assertEquals "Plain reproduce test $infile did not return same result" 0 $?

    sz1=$(ls -l res.xml | cut -d " " -f 5)

    opts=($eopts $arg1_opts $arg1_alt_opts)
    echo -n "Parse file $infile with '$opts'\r"
    p2x $opts -p ../../examples/configs/default $infile > res.xml
    xsltproc ../../src/xsl/$reprxsl res.xml > res.txt
    diff $infile res.txt
    assertEquals "Alternate opts reproduce test $infile did not return same result" 0 $?

    sz2=$(ls -l res.xml | cut -d " " -f 5)
    saving=$(( 1.0 * $sz1 / $sz2 ))
#    echo "saved: $sz1 / $sz2 = $saving"

    [[ $saving -gt 1 ]]
    assertEquals "Alternate XML format should be smaller or of same size: ($sz1 > $sz2)" 0 $?

    #        rm res.xml res.txt

}
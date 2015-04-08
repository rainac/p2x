#! /bin/sh

infile=$1
shift
opts=$*
p2x -p ../../examples/configs/default $opts $infile > res.xml
xsltproc -o res.txt ../../src/xsl/parens.xsl res.xml
cat res.txt
#rm res.txt res.xml

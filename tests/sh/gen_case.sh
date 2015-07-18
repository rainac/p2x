#! /bin/zsh

#set -x

TMP=${TMP:-/tmp}

infile=$TMP/p2x-speed-in
outfile=$TMP/p2x-out.xml

IN_KB=${IN_KB:-1} # how many MB of input data
WORD=${WORD:-1+} # what shall be repeated

P2X=${P2X:-p2x}

yes "$WORD" | dd of=$infile ibs=$(( ${#WORD} + 1))c cbs=${#WORD}c count=$(($IN_KB*1024/${#WORD})) conv=block 2> /dev/null

echo "1" >> $infile


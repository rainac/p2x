#! /bin/zsh

#set -x

TMP=${TMP:-/tmp}

infile=$TMP/p2x-speed-in
outfile=$TMP/p2x-out.xml

IN_KB=${IN_KB:-1} # how many MB of input data

P2X=${P2X:-p2x}

yes | dd of=$infile ibs=16c cbs=2c count=$((64*$IN_KB)) obs=1c conv=block,sync

ls -lh $infile
wc $infile

time $P2X $P2XOPTS -p ../../examples/configs/default -S c $infile > $outfile

ls -lh $outfile
wc $outfile

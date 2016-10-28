#! /bin/zsh

#set -x

TMP=${TMP:-/tmp}

P2XOPTS=($P2XOPTS -X -m -Vtimes,io)

infile=$TMP/p2x-speed-in
outfile=$TMP/p2x-out.xml

IN_KB=${IN_KB:-1000} # how many KB of input data

P2X=${P2X:-p2x}

yes | dd of=$infile ibs=16c cbs=2c count=$((64*$IN_KB)) obs=1c conv=block,sync

ls -lh $infile
szin=$(ls -l $infile | cut -d " " -f 5)
wc $infile

echo $P2X $P2XOPTS -p ../../examples/configs/default -S c $infile
time ($P2X $P2XOPTS -p ../../examples/configs/default -S c $infile > $outfile)

ls -lh $outfile
szout=$(ls -l $outfile | cut -d " " -f 5)
wc $outfile

echo "size ratio: $(( 1.0 * $szout / $szin))"

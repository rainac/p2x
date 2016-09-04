#! /bin/bash

set -x

SRC=$1
BRANCH=$2
outdir=$3

cd $HOME

if [[ "$SRC" = "http"* ]]; then
    if [[ -d "$outdir" ]]; then
        cd $outdir
        git reset --hard || exit 1
        git clean -x -f -d || exit 1
        git checkout $BRANCH || exit 1
        git pull || exit 1
    else
        git clone -b $BRANCH $SRC $outdir
    fi
else
    rm -rf $outdir
    git clone -b $BRANCH $SRC $outdir
fi

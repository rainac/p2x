#! /bin/bash

set -x

tarb=$1

tarb_name=$(basename $tarb)
tarb=$(readlink -f $tarb)

mydir=$(dirname $(readlink -f $BASH_SOURCE))

curdir=$PWD

version=${tarb_name#p2x-}
version=${version%-src.tar.gz}

if [[ -z "$P2X_DEVEL_REPO" ]]; then
    export P2X_DEVEL_REPO=$top_srcdir
fi
if [[ -z "$TMP" ]]; then
    export TMP=/tmp
fi

tmpd=$TMP/p2x-build-src-$BASHPID
mkdir $tmpd
cd $tmpd

build=$TMP/p2x-build-$BASHPID
mkdir $build

tar -xzf $tarb p2x-$version

cd p2x-$version

export SWPREFIX=$build/p2x-$version-$OSTYPE

$mydir/install-and-check.sh nocheck

cd $build

tar -cvzf $curdir/p2x-$version-$OSTYPE.tar.gz p2x-$version-$OSTYPE

rm -rf p2x-$version-$OSTYPE

cd ..

rm -r $build
rm -r $tmpd

cd $curdir
ls -lrt | tail -n 3

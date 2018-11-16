#! /bin/bash

dockerOS=${1:-debian}
dockerTag=${2:-stable}

mydir=$(dirname $(readlink -f $BASH_SOURCE))

curdir=$PWD

while ! [[ -f "src/p2x.cc" ]]; do
    cd ..
    if [[ "$PWD" = "/" ]]; then
	echo "Error: non in src tree"
	exit 1
    fi
done

top_srcdir=$PWD

version=$(cat $top_srcdir/version.txt)

if [[ -z "$P2X_DEVEL_REPO" ]]; then
    export P2X_DEVEL_REPO=$top_srcdir
fi
if [[ -z "$TMP" ]]; then
    export TMP=/tmp
fi

tmpd=$TMP/p2x-build-test-$BASHPID
mkdir $tmpd

cd $tmpd
mkdir p2x-src
cd p2x-src
echo "CLONE $P2X_DEVEL_REPO"
git clone $P2X_DEVEL_REPO

if [[ -z "$CXXFLAGS" ]]; then CXXFLAGS="-O3 -Wall"; fi
if [[ -z "$CPPFLAGS" ]]; then CPPFLAGS="-DNDEBUG"; fi
if [[ -z "$LDFLAGS" ]];  then LDFLAGS="-s"; fi

HOST=x86_64-w64-mingw32

mkdir $tmpd/p2x-src/include
$mydir/run-on-docker.sh -O $dockerOS -T $dockerTag -w $tmpd/p2x-src/include ln -s /usr/include/FlexLexer.h

WIN32CPPFLAGS="-I$tmpd/p2x-src/include"

$mydir/run-on-docker.sh -O $dockerOS -T $dockerTag -w $tmpd/p2x-src/p2x \
			./bootstrap
$mydir/run-on-docker.sh -O $dockerOS -T $dockerTag -w $tmpd/p2x-src/p2x \
			./configure --prefix=$tmpd/p2x-$version-windows --quiet --host=$HOST \
			"CXXFLAGS=$CXXFLAGS" "CPPFLAGS=$CPPFLAGS $WIN32CPPFLAGS" "LDFLAGS=$LDFLAGS"
$mydir/run-on-docker.sh -O $dockerOS -T $dockerTag -w $tmpd/p2x-src/p2x \
			make install

cd $tmpd
rm -rf $curdir/p2x-$version-windows.zip
zip -q -r $curdir/p2x-$version-windows.zip p2x-$version-windows

cd $tmpd
rm -rf p2x-src
rm -rf p2x-build
cd ..
rm -r $tmpd

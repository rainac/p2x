#! /bin/bash

set -x

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

version=$($top_srcdir/ci/scripts/get-version.sh)

if [[ -z "$P2X_DEVEL_REPO" ]]; then
    export P2X_DEVEL_REPO=$top_srcdir
fi
if [[ -z "$TMP" ]]; then
    export TMP=/tmp
fi

commit=$(git rev-parse HEAD)
commit=${commit:0:8}
branch=$(git rev-parse --abbrev-ref HEAD)

tmpd=$TMP/p2x-create-src-$BASHPID
mkdir $tmpd
cd $tmpd

if [[ "$branch" = "$master" ]]; then
    branch="-$commit"
else
    branch="-$commit-$branch"
fi
version=$version$branch

echo "CLONE $P2X_DEVEL_REPO"
git clone $P2X_DEVEL_REPO p2x-$version

cd p2x-$version
rm -rf .git
echo "$commit" > vcs-version.txt
cd ..

tar -czf $curdir/p2x-$version-src.tar.gz p2x-$version

cd $tmpd
rm -rf p2x-$version
cd ..
rm -r $tmpd

cd $curdir
ls -lrt | tail -n 3

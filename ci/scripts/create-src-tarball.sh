#! /bin/bash

set -x

mydir=$(dirname $(readlink -f $BASH_SOURCE))

curdir=$PWD

P2X_DEVEL_REPO=${1:-$P2X_DEVEL_REPO}

if [[ -z "$P2X_DEVEL_REPO" ]]; then
    while ! [[ -f "src/p2x.cc" ]]; do
        cd ..
        if [[ "$PWD" = "/" ]]; then
	    echo "Error: non in src tree"
	    exit 1
        fi
    done
    export P2X_DEVEL_REPO=$PWD
else
    P2X_DEVEL_REPO=$(readlink -f $P2X_DEVEL_REPO)
fi
if [[ -z "$TMP" ]]; then
    export TMP=/tmp
fi

cd $P2X_DEVEL_REPO

version=$($curdir/ci/scripts/get-version.sh)

commit=$(git rev-parse HEAD)
commit=${commit:0:8}
branch=$(git rev-parse --abbrev-ref HEAD)
branch=${branch/\//-}

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

pwd
ls -l

cd p2x-$version
rm -rf .git
echo "$commit" > vcs-version.txt
cd ..

pwd
ls -l

tar -czf $curdir/p2x-$version-src.tar.gz p2x-$version

cd $tmpd
rm -rf p2x-$version
cd ..
rm -r $tmpd

cd $curdir
ls -lrt | tail -n 3

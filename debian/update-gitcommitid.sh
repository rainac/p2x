#! /bin/bash

cd $(dirname $GBP_GIT_DIR)

COMMITID=$(./vcs-version.sh)

cd $GBP_BUILD_DIR

echo "$COMMITID" > vcs-version.txt

dpkg-source --commit

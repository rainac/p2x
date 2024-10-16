#! /bin/bash

# P2X shunit2 test suite: installation scripts
#
# This suite tries to checkout the source and install P2X.

export LANG=C # for grep used in shunit2, depends on english output
export TMP=${TMP:-/tmp}

P2X_SOURCE=${P2X_SOURCE:-$(readlink -f ../..)}

runInstallation() {
    branch=$1

    rm -rf p2x-tmp

    git clone $P2X_SOURCE p2x-tmp
    assertEquals "GIT clone of P2X must succeed" 0 $?

    cd p2x-tmp
    assertEquals "CD run must succeed" 0 $?

    git checkout $branch
    assertEquals "GIT checkout of branch $branch must succeed" 0 $?

    ./bootstrap

    ./configure --quiet --prefix=$TMP/p2x-build-test
    assertEquals "CONFIGURE run must succeed" 0 $?

    make -s -j8 clean
    assertEquals "MAKE CLEAN run must succeed" 0 $?

    make -s -j8 install
    assertEquals "MAKE run must succeed" 0 $?

    test -x $TMP/p2x-build-test/bin/p2x
    assertEquals "P2X executable must be installed" 0 $?

    $TMP/p2x-build-test/bin/p2x --version
    assertEquals "P2X executable must run" 0 $?

    cd ..
    rm -rf p2x-tmp
    rm -rf $TMP/p2x-build-test
}

testP2X_clean_install_master() {
    runInstallation master
}

testP2X_clean_install_current_branch() {
    curBranch=$(git branch | grep '*' | awk '{ print $2 }')
    runInstallation $curBranch
}

. shunit2

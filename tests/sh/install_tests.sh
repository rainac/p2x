#! /bin/zsh

# P2X shunit2 test suite: installation scripts
#
# This suite tries to checkout the source and install P2X.

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output
export TMP=${TMP:-/tmp}

P2X_SOURCE=${P2X_SOURCE:-https://github.com/rainac/p2x.git}

testP2X_clean_install_master() {

    rm -rf p2x-tmp

    git clone $P2X_SOURCE p2x-tmp
    assertEquals "git clone of P2X must succeed" 0 $?

    cd p2x-tmp
    assertEquals "P2X run must succeed" 0 $?

    ./configure --quiet --prefix=$TMP
    assertEquals "P2X configure run must succeed" 0 $?

    make -s -j8 install
    assertEquals "P2X make run must succeed" 0 $?

    make git-clean
    assertEquals "P2X make run must succeed" 0 $?

    make -s -j8 install
    assertEquals "P2X make run must succeed" 0 $?

    rm -rf p2x-tmp
}

testP2X_clean_install_current_branch() {

    curBranch=$(git branch | grep '*' | awk '{ print $2 }')

    rm -rf p2x-tmp

    git clone $P2X_SOURCE p2x-tmp
    assertEquals "git clone of P2X must succeed" 0 $?

    cd p2x-tmp
    assertEquals "P2X run must succeed" 0 $?

    git checkout $curBranch
    assertEquals "P2X checkout must succeed" 0 $?

    ./configure --quiet --prefix=$TMP
    assertEquals "P2X configure run must succeed" 0 $?

    make -s -j8 install
    assertEquals "P2X make run must succeed" 0 $?

    make git-clean
    assertEquals "P2X make run must succeed" 0 $?

    make -s -j8 install
    assertEquals "P2X make run must succeed" 0 $?

    rm -rf p2x-tmp
}

. shunit2

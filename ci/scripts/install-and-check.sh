#! /bin/bash

mydir=$(dirname $BASH_SOURCE)

make clean
./bootstrap
./do_config.sh
make -j 8
sudo make install
make check

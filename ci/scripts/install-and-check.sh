#! /bin/bash

mydir=$(dirname $BASH_SOURCE)

./bootstrap
./do_config.sh
make -j 8
sudo make install
make check
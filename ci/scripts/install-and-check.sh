#! /bin/bash

mydir=$(dirname $BASH_SOURCE)

./bootstrap
./do_config.sh
make -j 8
make install prefix=$SWPREFIX

if [[ "$1" = "nocheck" ]]; then
   :
else
       make check
fi

#! /bin/bash

# Software prefix $SWPREFIX defaults to /usr/local
SWPREFIX=${SWPREFIX:-/usr/local}

# Invoke configure
export CFLAGS="$CFLAGS -Wall -Wextra"
export CXXFLAGS="$CXXFLAGS -Wall -Wextra"
export LDFLAGS="$LDFLAGS -Wall -Wextra"

./configure --enable-tests --prefix=$SWPREFIX/p2x-0.5.2

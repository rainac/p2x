#! /bin/bash

# Software prefix $PREFIX is set to /usr/local, if $SWPREFIX is not set
# it is set to $SWPREFIX/p2x-0.5.2 otherwise


if [[ -z "$SWPREFIX" ]]; then
    PREFIX=/usr/local
else
    PREFIX=$SWPREFIX/p2x-0.5.2
fi

# Invoke configure
export CFLAGS="$CFLAGS -Wall -Wextra"
export CXXFLAGS="$CXXFLAGS -Wall -Wextra"
export LDFLAGS="$LDFLAGS -Wall -Wextra"

./configure --enable-tests --prefix=$PREFIX $@

#! /bin/bash

sed -e '/^ *\/\// D' < $1 \
    | sed -e '/^ *$/ D' \
    | sed -e 's/^ *//' \
    | sed -e '/require/ s/\.js/.min.js/' \
    | sed -e 's/ *$//' \
    | sed -e 's/ *\(+\|=\|+=\|==\|!=\|&&\|{\|}\|(\|)\|:\|,\|-\) */\1/g'

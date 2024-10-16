#! /bin/bash

set -e -x

mkdir -p ccode

split -l 1 ../../examples/in/cdecls.exp ccode/

for cfile in ccode/*; do
    mv $cfile $cfile.c
    gcc -o $cfile.o -c $cfile.c
    if [[ $? != "0" ]]; then
        echo "Compiler error"
    fi
done

rm -rf ccode

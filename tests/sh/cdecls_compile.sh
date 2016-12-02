#! /bin/zsh

mkdir ccode

split -l 1 ../../examples/in/cdecls.exp ccode/

for cfile in ccode/*; do
    mv $cfile $cfile.c
    gcc -c $cfile.c
    if [[ $? != "0" ]]; then
        echo "Compiler error"
    fi
done

rm -rf ccode

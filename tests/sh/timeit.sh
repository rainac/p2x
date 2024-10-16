#/bin/bash

t0=$(date +%s.%N)

echo running $1 >&2
bash -c "$1"

t1=$(date +%s.%N)
ttime=$(python -c "print(${t1} - ${t0})")
# ttime=$(echo "$t1 $t0 - p" | dc)
echo "$ttime"

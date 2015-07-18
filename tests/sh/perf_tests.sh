#! /bin/zsh

# P2X shunit2 test suite: performace tests
#
# This suite runs some tests timing p2x runs on different inputs.

zmodload zsh/mathfunc
set -o shwordsplit
echo $0
export SHUNIT_PARENT=$0
export LANG=C # for grep used in shunit2, depends on english output

P2X_OPTS=(--indent -bPLUS -rEQUAL -iSPACE)
KB_LIST=(1 2 4 8 16 32 64)

# Test section performance

testP2_check_test_case_generation() {
    for kb in 1 10 20; do
        IN_KB=$kb zsh gen_case.sh
        assertEquals "$(ls -l /tmp/p2x-speed-in | awk '{ printf $5 }')" "$(( $kb * 1024 +2 ))"
    done
}

testP2_check_test_case_generation2() {
    for kb in 1 10 20; do
        IN_KB=$kb WORD="1=  " zsh gen_case.sh
        assertEquals "$(ls -l /tmp/p2x-speed-in | awk '{ printf $5 }')" "$(( $kb * 1024 +2 ))"
    done
}

runTest() {
    IN_KB=$1
    WORD=$2

    IN_KB=$IN_KB WORD="$WORD" zsh gen_case.sh
    #        ls -lh /tmp/p2x-speed-in
    inpsz=$(ls -l /tmp/p2x-speed-in | awk '{ printf $5 }')

    (time (p2x $P2X_OPTS /tmp/p2x-speed-in > /tmp/p2x-out.xml 2> err.txt) ) 2> terr.txt > log.txt
    #        cat err.txt

    ttime=$(awk '{ print $7 }' < terr.txt)

    echo "$inpsz $ttime" >> $times_file

    rm /tmp/p2x-speed-in
    rm /tmp/p2x-out.xml
}

testP2_perf_plus_not_too_bad() {
    times_file=results_plus.txt
    truncate --size 0 $times_file
    for kb in $KB_LIST; do
        runTest $kb "1+"
    done
    octave --quiet <<EOF
v = load('$times_file');
Bps = v(:,1) ./ v(:,2)
min(Bps)
mean(Bps)
max(Bps)
assert(mean(Bps) < min(Bps)*5)
assert(max(Bps) < mean(Bps)*5)
EOF
    assertEquals "performance should be relatively equal" "0" "$?"
}

testP2_perf_eq_not_too_bad() {
    times_file=results_eq.txt
    truncate --size 0 $times_file
    for kb in $KB_LIST; do
        runTest $kb "1="
    done
    octave --quiet <<EOF
v = load('$times_file');
Bps = v(:,1) ./ v(:,2)
min(Bps)
mean(Bps)
max(Bps)
assert(mean(Bps) < min(Bps)*5)
assert(max(Bps) < mean(Bps)*5)
EOF
    assertEquals "performance should be relatively equal" "0" "$?"
}

testP2_perf_eqs_not_too_bad() {
    times_file=results_eqs.txt
    truncate --size 0 $times_file
    for kb in $KB_LIST; do
        runTest $kb "1= "
    done
    octave --quiet <<EOF
v = load('$times_file');
Bps = v(:,1) ./ v(:,2)
min(Bps)
mean(Bps)
max(Bps)
assert(mean(Bps) < min(Bps)*5)
assert(max(Bps) < mean(Bps)*5)
EOF
    assertEquals "performance should be relatively equal" "0" "$?"
}

plotResult() {
    inFile=$1
    octave --quiet <<EOF
graphics_toolkit("gnuplot")
v = load('$inFile');
loglog(v(:,1), v(:,2), '-+')
xlabel('input size (B)')
ylabel('time (s)')
title('very long input "$inFile"')
set(gca, 'ylim', [1e-3 1e2])
print -dpng $(basename $inFile .txt).png
EOF
}

testP2_plot_results() {
    plotResult results_plus.txt
    plotResult results_eq.txt
    plotResult results_eqs.txt
}

testP2_cleanup() {
    rm results_plus.txt results_eq.txt results_eqs.txt
    rm err.txt terr.txt log.txt
}

. shunit2

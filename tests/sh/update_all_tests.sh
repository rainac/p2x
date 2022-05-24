#! /bin/bash

cat header_all_tests.sh > all_tests.sh

let n=0
while test -n "$1"; do
    cat >> all_tests.sh <<EOF
test_suite_$n() {
  run_suite $1.sh
}

EOF
    shift
    let n=n+1
done

cat >> all_tests.sh <<EOF
. shunit2
EOF

chmod a+x all_tests.sh

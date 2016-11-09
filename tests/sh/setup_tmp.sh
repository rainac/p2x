tmpdir=${TMP:-/tmp}/shunit2-test-$$
mkdir $tmpdir

function mycleanup() {
#    echo "CLEANUP"
    rm -rf $tmpdir
}

export SHUNIT_EXIT=mycleanup

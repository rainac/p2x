#! /bin/bash

echo "# automatically generated" > forall_tests.sh
echo "## begin all tests list" >> forall_tests.sh
for i in ../../examples/in/*.exp; do
    echo "test_$(basename $i .exp | tr -d '.\-+' )() {" >> forall_tests.sh
    echo "checkExpFile $(basename $i)" >> forall_tests.sh
    echo "}" >> forall_tests.sh
    echo "" >> forall_tests.sh
done
echo "## end all tests list" >> forall_tests.sh

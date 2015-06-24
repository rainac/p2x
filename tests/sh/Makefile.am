
generated_files = test_cmp_js.sh test_reparse.sh test_reproduce_strict.sh test_reproduce.sh test_output_stability.sh forall_tests.sh

forall_tests.sh: update_forall_tests.sh ../../examples/in
	./update_forall_tests.sh

test_%.sh: head_%.sh forall_tests.sh
	cat $< forall_tests.sh > $@
	echo ". shunit2" >> $@
	chmod a+x $@

all: $(generated_files)

test-log.txt: all
	echo running tests ...
	-./all_tests.sh > $@ 2>&1
	echo done running tests

test-report.html: test-log.txt
	grep -E "(^Ran|^FAILED|^OK|^ASSERT)" $< | tee $@

check: test-report.html

install: all

clean:
	$(RM) -f $(generated_files) test-log.txt test-report.html

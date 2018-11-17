
generated_files = test_cmp_js.sh test_cmp_jsy.sh test_reparse.sh test_reproduce_strict.sh test_reproduce.sh test_output_stability.sh forall_tests.sh

good = val_tests test_reproduce_strict test_reproduce test_reparse	\
parse_tests merge_tests p2x_tests test_cmp_js p2xjs_mocha_tests		\
test_output_stability indent_tests reproduce_ign valgrind_tests		\
output_mode_json_merged output_mode_json output_mode_matlab_merged	\
output_mode_matlab

bad = false_test

good_test_results = $(addprefix result_suite_good_,$(good))
bad_test_results = $(addprefix result_suite_bad_,$(bad))

result_suite_good_%: %.sh
	./$< > log_$*.txt 2>&1

result_suite_bad_%: %.sh
	! ./$< > log_$*.txt 2>&1

run_%: test_%.sh
	./$<

forall_tests.sh: update_forall_tests.sh ../../examples/in
	./update_forall_tests.sh

test_%.sh: head_%.sh forall_tests.sh
	cat $< forall_tests.sh > $@
	echo ". ./myshunit2" >> $@
	chmod a+x $@

all: $(generated_files)

test-log.txt: all
	echo running tests ...
	-./all_tests.sh > $@ 2>&1
	echo done running tests

test-report.html: test-log.txt
	grep -E "(^Ran|^FAILED|^OK|^ASSERT)" $< | tee $@

check: check-good check-bad

check-good: $(good_test_results)

check-bad: $(bad_test_results)

install: all

clean:
	$(RM) -f $(generated_files) test-log.txt test-report.html log_*.txt
	rm -f ok.txt fail.txt

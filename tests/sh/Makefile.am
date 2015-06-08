
generated_files = test_cmp_js.sh forall_tests.sh

forall_tests.sh: update_forall_tests.sh ../../examples/in
	./update_forall_tests.sh

test_%.sh: head_%.sh forall_tests.sh
	cat $< forall_tests.sh > $@
	echo ". shunit2" >> $@
	chmod a+x $@

all: $(generated_files)

check: all
	./all_tests.sh

install: all

clean:
	$(RM) -f $(generated_files)

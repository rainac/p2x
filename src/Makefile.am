#
# This file is part of P2X.
# Copyright © 2013 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

bin_PROGRAMS = p2x
if acond_BUILD_TESTS
bin_PROGRAMS += p2xtests
endif

p2x_FLEX_SRCS = scanner.ll scanner-no-strings.ll scanner-m.ll scanner-r.ll scanner-c.ll
p2x_SOURCES = p2x.cc $(p2x_FLEX_SRCS) p2x-opts.cc token.cc logger.cc
p2xtests_SOURCES = p2xtests.cc scanner.ll scanner-no-strings.ll p2x-opts.cc token.cc logger.cc

p2x_CXXFLAGS =
p2x_LDADD = 
p2x_LDFLAGS =
p2x_DEPENDENCIES =

p2xtests_LDADD = -lcppunit

generated_files = namespaces.hh code-xml.rnc
if acond_GENGETOPT_AVAIL
generated_files += p2x-opts.hh p2x-opts.cc
endif
if acond_TRANG_AVAIL
generated_files += code-xml.rng
endif
if acond_FLEX_AVAIL
generated_files += $(patsubst %.ll,%.cc,$(p2x_FLEX_SRCS))
endif

p2x.cc: $(generated_files)

if acond_GENGETOPT_AVAIL
# The headerfile of the commandline arguments parser is generated from
# the .ggo file.
p2x-opts.hh: p2x-opts.cc
p2x-opts.cc: p2x-opts.ggo
	$(GENGETOPT) -F $(*F) -c cc -H hh -C --no-handle-version \
               --no-handle-help --no-handle-error -i $< --unamed-opts
endif

code-xml.rng: code-xml.rnc
	$(TRANG) $< $@

flexPrefix = scan

scanner-no-strings.cc: flexPrefix = scanNS
scanner-r.cc: flexPrefix = scanR
scanner-m.cc: flexPrefix = scanM
scanner-c.cc: flexPrefix = scanC

# scanner-no-strings.ll is produced by deleting the line which matches strings
scanner-no-strings.ll: scanner.ll
	sed -e '/{STRING}/ c \ ' < $< > $@

%.cc: %.ll
	flex -P$(flexPrefix) -o$@ $<


namespaces.hh: namespace-cx namespace-ca
	echo "#define NAMESPACE_CX \"$(shell cat namespace-cx)\"" > $@
	echo "#define NAMESPACE_CA \"$(shell cat namespace-ca)\"" >> $@

code-xml.rnc: code-xml.rnc.in
	sed -e "s?NAMESPACE_CX?$(shell cat namespace-cx)?" -e "s?NAMESPACE_CA?$(shell cat namespace-ca)?" $< > $@

instbasedir=$(pkgdatadir)
instdir= $(instbasedir)

inst_DATA = code-xml.rnc

clean-local:
	$(RM) -f $(generated_files)

distclean-local:

check:
	./p2xtests

#
# This file is part of P2X.
# Copyright © 2013 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

SUBDIRS = src src-js examples tests

project_files= AUTHORS COPYING ChangeLog INSTALL README.txt TODO NEWS	\
version.txt

EXTRA_DIST = aclocal.m4 configure.ac

instdir = $(datadir)/doc/$(PACKAGE)

inst_DATA= $(project_files)

install-recursive all-recursive: svn-version.txt

svn-version.txt: .
	git log -n 1 | head -1 | awk '{print $$2}' > $@.tmp; \
	if ! diff $@.tmp $@ > /dev/null; then cp $@.tmp $@; else echo "$@ is unchanged"; fi; \
	$(RM) $@.tmp

version.txt:
	echo "$(VERSION)" > $@

distclean-local:
	$(RM) -f svn-version.txt version.txt
	$(RM) -rf autom4te.cache

METASOURCES = AUTO

#
# This file is part of P2X.
# Copyright © 2013 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

SUBDIRS = src examples tests

project_files= AUTHORS COPYING ChangeLog INSTALL README.txt TODO NEWS	\
version.txt

EXTRA_DIST = aclocal.m4 configure.ac

instdir = $(datadir)/doc/$(PACKAGE)

inst_DATA= $(project_files)

install-recursive all-recursive: svn-version.txt

svn-version.txt: . 
	if test `svnversion . | $(GHEAD) -c 4` = "expo"; then \
	   echo "files are exported"; \
	else \
	   svnversion . > $@.tmp; \
	   if ! diff $@.tmp $@ > /dev/null; then cp $@.tmp $@; else echo "$@ is unchanged"; fi; \
	   $(RM) $@.tmp; \
	fi

version.txt:
	echo "$(VERSION)" > $@

distclean-local:
	$(RM) -f svn-version.txt version.txt
	$(RM) -rf autom4te.cache

METASOURCES = AUTO

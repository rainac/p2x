#
# This file is part of P2X.
# Copyright © 2013 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

SUBDIRS = src src-js examples tests

generated_files = README.txt README.html README.pdf

project_files= AUTHORS COPYING ChangeLog INSTALL TODO NEWS	\
version.txt $(generated_files)

EXTRA_DIST = aclocal.m4 configure.ac

instdir = $(datadir)/doc/$(PACKAGE)

inst_DATA= $(project_files)

install-recursive all-recursive: svn-version.txt

if acond_EMACS_AVAIL
README.pdf: README.org
	emacs -q --batch --file $< -f org-latex-export-to-pdf
	rm -f README.tex

README.html: README.org
	emacs -q --batch --file $< -f org-html-export-to-html

README.txt: README.org
	emacs -q --batch --file $< -f org-ascii-export-to-ascii
endif

svn-version.txt: .
	./vcs-version.sh > $@.tmp; \
	if ! diff $@.tmp $@ > /dev/null; then cp $@.tmp $@; else echo "$@ is unchanged"; fi; \
	$(RM) $@.tmp

version.txt:
	echo "$(VERSION)" > $@

clean-local:

distclean-local:
	$(RM) -f svn-version.txt version.txt
	$(RM) -rf autom4te.cache
if acond_EMACS_AVAIL
	$(RM) -f $(generated_files)
endif

git-clean:
	git clean -d

METASOURCES = AUTO

#
# This file is part of P2X.
# Copyright © 2013 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

SUBDIRS = examples tests
if acond_CPLUSPLUS_ENABLED
SUBDIRS += src
endif
if acond_NPM_AVAIL
SUBDIRS += src-js
endif

generated_files = README.txt README.html README.pdf

project_files= AUTHORS COPYING ChangeLog INSTALL TODO NEWS	\
version.txt $(generated_files)

EXTRA_DIST = aclocal.m4 configure.ac

instdir = $(docdir)

inst_DATA= $(project_files)

bin_SCRIPTS = p2x-show-tree

install-recursive all-recursive: vcs-version.txt

if acond_EMACS_AVAIL
README.pdf: README.org
	emacs -q --batch --file $< -f org-latex-export-to-pdf
	rm -f README.tex

README.html: README.org
	emacs -q --batch --file $< -f org-html-export-to-html

README.txt: README.org
	emacs -q --batch --file $< -f org-ascii-export-to-ascii
endif

vcs-version.txt: .
	./vcs-version.sh > $@.tmp; \
	if ! diff $@.tmp $@ > /dev/null; then cp $@.tmp $@; else echo "$@ is unchanged"; fi; \
	$(RM) $@.tmp

version.txt:
	echo "$(VERSION)" > $@

clean-local:

distclean-local:
	$(RM) -f vcs-version.txt version.txt
	$(RM) -rf autom4te.cache
if acond_EMACS_AVAIL
	$(RM) -f $(generated_files)
endif

git-clean:
	git clean -d

p2x-show-tree-check-local:
	echo -e "double x[3] = {1,2,3};" | P2X_XSL=src/xsl P2X_CONFIG=examples/configs ./p2x-show-tree -x -X -x -n -x -m -g src/xml/showX.graphotron.xml -

METASOURCES = AUTO

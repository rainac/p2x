#
# This file is part of P2X.
# Copyright © 2014 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

instdir = $(DESTDIR)$(docdir)/examples

install:
	echo "prefix=$(prefix)"
	echo "bindir=$(bindir)"
	mkdir -p $(instdir)/configs
	mkdir -p $(instdir)/in
	mkdir -p $(instdir)/xsl
	install -c -m 644 configs/cdecls $(instdir)/configs
	install -c -m 644 in/cdecls.exp  $(instdir)/in
	install -c -m 644 xsl/cdecls.xsl $(instdir)/xsl

examples = $(wildcard in/*.exp)

exampleoutput = $(addsuffix .xml, $(addprefix out/, $(basename $(notdir $(examples)))))

p2xflags = 

out/%.xml: in/%.exp
	$(top_srcdir)/src/p2x $(p2xflags) -p $(top_srcdir)/examples/configs/default -o $@ $<

out/letter.xml: p2xflags = -e latin1
out/german.xml: p2xflags = -e latin1
out/fliesst.xml: p2xflags = -e latin1

update-output: $(exampleoutput)

force-update-output:
	touch in/*.exp
	make update-output

clean-output:
	$(RM) -f out/*.xml

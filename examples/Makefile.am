#
# This file is part of P2X.
# Copyright © 2014 Johannes Willkomm 
# See the file p2x.cc for copying conditions.  
#

instbasedir=$(pkgdatadir)
instdir= $(instbasedir)

examples = $(wildcard in/*.exp)

exampleoutput = $(addsuffix .xml, $(addprefix out/, $(basename $(notdir $(examples)))))

p2xflags = 

out/%.xml: in/%.exp
	$(top_srcdir)/src/p2x $(p2xflags) -p $(top_srcdir)/examples/configs/default < $< | xsltproc $(top_srcdir)/src/xsl/but-root.xsl - > $@

out/letter.xml: p2xflags = -e latin1
out/german.xml: p2xflags = -e latin1
out/fliesst.xml: p2xflags = -e latin1

update-output: $(exampleoutput)

clean-output:
	$(RM) -f out/*.xml

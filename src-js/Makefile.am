
generated_files = scanner-setup.js token.ncd.js modes.ncd.js assoc.ncd.js

instbasedir=$(pkgdatadir)
instdir= $(instbasedir)

inst_DATA = $(generated_files)

%.ncd.js: $(top_srcdir)/src/xml/%.ncd.xml
	gennc -m JavaScript $^

scanner-setup.js: $(top_srcdir)/src/xsl/gen-js-scanner.xsl $(top_srcdir)/src/scanner.xml
	xsltproc -o $@ $^

clean-local:
	$(RM) -f $(generated_files)




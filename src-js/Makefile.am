
generated_files = token.ncd.js modes.ncd.js assoc.ncd.js

instbasedir=$(pkgdatadir)
instdir= $(instbasedir)/js

dist_bin_SCRIPTS = p2xjs

project_files = $(generated_files) scanner.js parse-xml.js parse-opts.js p2x.js p2x-main.js scanconf-xml2json.js parseconf-text2json.js

inst_DATA = $(project_files)

%.ncd.js: $(top_srcdir)/src/xml/%.ncd.xml
	gennc -m JavaScript $^

scanner-setup.js: $(top_srcdir)/src/xsl/gen-js-scanner.xsl $(top_srcdir)/src/scanner.xml
	xsltproc -o $@ $^

clean-local:
	$(RM) -f $(generated_files)

apache_confd=$(sysconfdir)/apache2/conf.d

install-data-local:
	cd $(instdir) && npm link most-xml
	$(INSTALL) -d $(apache_confd)
	echo "Alias $(server_base)/p2x $(prefix)/share/p2x/js/" > $(apache_confd)/p2x-js.conf

#install-data-local:
#	cp -r node_modules $(instdir)/node_modules

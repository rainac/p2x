
json_files = scanner-c.json
generated_files = token.ncd.js modes.ncd.js assoc.ncd.js $(json_files)

instbasedir=$(pkgdatadir)
instdir= $(instbasedir)/js

dist_bin_SCRIPTS = p2xjs

project_files = $(generated_files) scanner.js hashmap.js parse-xml.js parse-opts.js p2x.js p2x-main.js scanconf-xml2json.js parseconf-text2json.js p2xa.js

inst_DATA = $(project_files)

%.ncd.js: $(top_srcdir)/src/xml/%.ncd.xml
	gennc -m JavaScript $^

%.json: $(top_srcdir)/src/xml/%.xml $(node_modules)
	./p2xjs scanconf-xml2json -S $< > $@

p2xa.js: modes.ncd.js token.ncd.js assoc.ncd.js parse-xml.js hashmap.js scanner.js
	cat $^ > $@

scanner-setup.js: $(top_srcdir)/src/xsl/gen-js-scanner.xsl $(top_srcdir)/src/scanner.xml
	xsltproc -o $@ $^

clean-local:
	$(RM) -f $(generated_files)

apache_confd=$(sysconfdir)/apache2/conf.d

node_module_names = most-xml tmp

node_modules = $(addprefix node_modules/, $(node_module_names))

.PRECIOUS: node_modules/%

node_modules/%:
	npm link $*

$(instdir):
	$(INSTALL) -d $@

install-data-local: $(instdir)
	cd $(instdir) $(foreach f, $(node_module_names), && npm link $(f))
	$(INSTALL) -d $(apache_confd)
	echo "Alias $(server_base)/p2x $(prefix)/share/p2x/js/" > $(apache_confd)/p2x-js.conf

#install-data-local:
#	cp -r node_modules $(instdir)/node_modules

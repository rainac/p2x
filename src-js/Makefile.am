
json_files = scanner-c.json

instbasedir=$(pkgdatadir)
instdir= $(instbasedir)/js

dist_bin_SCRIPTS = p2xjs

ncd_files = token.ncd.js modes.ncd.js assoc.ncd.js

unmin_files = $(ncd_files) scanner.js hashmap.js parse-xml.js parse-opts.js p2x.js p2x-main.js scanconf-xml2json.js parseconf-text2json.js p2xa.js gennc-common.js p2x-tools.js

min_files = $(addsuffix .min.js, $(basename $(unmin_files)))

generated_files = $(min_files) $(json_files)

project_files = $(min_files)

inst_DATA = $(project_files)

gennc-common.js: token.ncd.js

%.ncd.js: $(top_srcdir)/src/xml/%.ncd.xml
	gennc -m JavaScript $^

%.json: $(top_srcdir)/src/xml/%.xml $(node_modules)
	./p2xjs scanconf-xml2json -S $< > $@

%.min.js: %.js kill-comments-empty.sh
	./kill-comments-empty.sh $< > $@


p2xa.js: gennc-common.min.js modes.ncd.min.js token.ncd.min.js assoc.ncd.min.js parse-xml.min.js hashmap.min.js scanner.min.js
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

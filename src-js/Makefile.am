
json_files = scanner-c.json

instbasedir = $(pkgdatadir)
instdir = $(instbasedir)/js

dist_bin_SCRIPTS = p2xjs

ncd_files = token.ncd.js modes.ncd.js assoc.ncd.js

unmin_files = $(ncd_files) scanner.js hashmap.js parse-xml.js parse-opts.js p2x.js p2xa.js p2x-main.js scanconf-xml2json.js parseconf-text2json.js gennc-common.js p2x-tools.js compress.js

min_files = $(addsuffix .min.js, $(basename $(unmin_files)))

generated_files = $(json_files) node_modules_updated.txt

project_files = $(json_files) $(unmin_files)

if acond_NPM_PKGS_AVAIL
generated_files += $(min_files)
project_files += $(min_files)
endif

inst_DATA = $(project_files)

gennc-common.js: token.ncd.js

%.ncd.js: $(top_srcdir)/src/xml/%.ncd.xml
	gennc -m JavaScript $^

%.json: ../examples/configs/%.json
	cp $< $@

%.min.js: %.js compress.js node_modules_updated.txt
	sed -e '/require/ s/.js/.min.js/' $< > $<.tmp
	nodejs compress.js -o $@ $<.tmp
	rm $<.tmp

p2xa.js: gennc-common.js modes.ncd.js token.ncd.js assoc.ncd.js hashmap.js scanner.js
	cat $^ > $@

scanner-setup.js: $(top_srcdir)/src/xsl/gen-js-scanner.xsl $(top_srcdir)/src/scanner.xml
	xsltproc -o $@ $^

clean-local:
	$(RM) -f $(generated_files)

apache_confd = $(DESTDIR)$(sysconfdir)/apache2/conf.d

node_module_names = most-xml tmp node-minify

node_modules = $(addprefix node_modules/, $(node_module_names))

.PRECIOUS: node_modules/%

node_modules/%: node_modules_updated.txt

node_modules:
	mkdir $@

$(node_modules) node_modules_updated.txt: node_modules Makefile.am
	touch node_modules_updated.txt
if acond_NPM_PKGS_AVAIL
	npm link $(node_module_names)
endif

install-data-local:
if acond_NPM_PKGS_AVAIL
	cd $(instdir) $(foreach f, $(node_module_names), && npm link $(f))
endif
	$(INSTALL) -d $(apache_confd)
	echo "Alias $(server_base)/p2x $(prefix)/share/p2x/js/" > $(apache_confd)/p2x-js.conf

#install-data-local:
#	cp -r node_modules $(instdir)/node_modules

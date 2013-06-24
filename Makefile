# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

PREFIX = /usr
DATA = /share
BIN = /bin
LIBEXEC = /libexec
PKGNAME = mongoclock
COMMAND = mongoclock
LICENSES = $(PREFIX)$(DATA)


all: mongoclock.install doc

doc: info

info: mongoclock.info.gz

%.info.gz: info/%.texinfo
	makeinfo "$<"
	gzip -9 -f "$*.info"

mongoclock.install: mongoclock
	cp "$<" "$@"
	sed -i 's:$$0.py:$(DESTDIR)$(PREFIX)$(LIBEXEC)/$(COMMAND).py:g' "$@"

install: install-cmd install-license install-info

install-cmd:
	install -dm755 "$(DESTDIR)$(PREFIX)$(BIN)"
	install -dm755 "$(DESTDIR)$(PREFIX)$(LIBEXEC)"
	install -m755 mongoclock.install "$(DESTDIR)$(PREFIX)$(BIN)/$(COMMAND)"
	install -m755 mongoclock.py "$(DESTDIR)$(PREFIX)$(LIBEXEC)/$(COMMAND).py"

install-license:
	install -dm755 "$(DESTDIR)$(LICENSES)/$(PKGNAME)"
	install -m644 COPYING LICENSE "$(DESTDIR)$(LICENSES)/$(PKGNAME)"

install-info: mongoclock.info.gz
	install -dm755 "$(DESTDIR)$(PREFIX)$(DATA)/info"
	install -m644 mongoclock.info.gz "$(DESTDIR)$(PREFIX)$(DATA)/info/$(PKGNAME).info.gz"

uninstall:
	-rm -- "$(DESTDIR)$(PREFIX)$(BIN)/$(COMMAND)"
	-rm -- "$(DESTDIR)$(PREFIX)$(LIBEXEC)/$(COMMAND).py"
	-rm -- "$(DESTDIR)$(LICENSES)/$(PKGNAME)/COPYING"
	-rm -- "$(DESTDIR)$(LICENSES)/$(PKGNAME)/LICENSE"
	-rmdir -- "$(DESTDIR)$(LICENSES)/$(PKGNAME)"
	-rm -- "$(DESTDIR)$(PREFIX)$(DATA)/info/$(PKGNAME).info.gz"

.PHONY: clean
clean:
	-rm -f mongoclock.info.gz mongoclock.install


# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

PREFIX = /usr
BIN = /bin
DATA = /share
BINDIR = $(PREFIX)$(BIN)
DATADIR = $(PREFIX)$(DATA)
DOCDIR = $(DATADIR)/doc
INFODIR = $(DATADIR)/info
MANDIR = $(DATADIR)/man
MAN1DIR = $(MANDIR)/man1
LICENSEDIR = $(DATADIR)/licenses

PKGNAME = mongoclock
COMMAND = mongoclock

GPP = gpp


WARN = -Wall -Wextra -pedantic -Wdouble-promotion -Wformat=2 -Winit-self -Wmissing-include-dirs      \
       -Wtrampolines -Wmissing-prototypes -Wmissing-declarations -Wnested-externs                    \
       -Wno-variadic-macros -Wsync-nand -Wunsafe-loop-optimizations -Wcast-align                     \
       -Wdeclaration-after-statement -Wundef -Wbad-function-cast -Wwrite-strings -Wlogical-op        \
       -Wstrict-prototypes -Wold-style-definition -Wpacked -Wvector-operation-performance            \
       -Wunsuffixed-float-constants -Wsuggest-attribute=const -Wsuggest-attribute=noreturn           \
       -Wsuggest-attribute=format -Wnormalized=nfkc -Wshadow -Wredundant-decls -Winline -Wcast-qual  \
       -Wsign-conversion -Wstrict-overflow=5 -Wconversion -Wsuggest-attribute=pure -Wswitch-default  \
       -Wstrict-aliasing=1 -fstrict-overflow -Wfloat-equal -Wpadded -Waggregate-return               \
       -Wtraditional-conversion

FFLAGS = -fstrict-aliasing -fipa-pure-const -ftree-vrp -fstack-usage -funsafe-loop-optimizations
STD = c99
OPTIMISE = -O2

FLAGS = $(OPTIMISE) -std=$(STD) $(WARN) $(FFLAGS)



.PHONY: default
default: command info

.PHONY: all
all: command doc

.PHONY: base
base: command

.PHONY: command
command: bin/mongoclock

bin/mongoclock: obj/mongoclock.o
	mkdir -p bin
	$(CC) $(FLAGS) -o $@ $^ $(LDFLAGS)

obj/mongoclock.o: obj/mongoclock.c
	$(CC) $(FLAGS) -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

obj/mongoclock.c: src/mongoclock.c
	mkdir -p obj
	$(GPP) -s '$$' -i $< -o $@

.PHONY: doc
doc: info pdf dvi ps

.PHONY: info
info: bin/mongoclock.info

.PHONY: info
info: bin/mongoclock.info
bin/%.info: doc/info/%.texinfo
	@mkdir -p bin
	$(MAKEINFO) $<
	mv $*.info $@

.PHONY: pdf
pdf: bin/mongoclock.pdf
bin/%.pdf: doc/info/%.texinfo
	@! test -d obj/pdf || rm -rf obj/pdf
	@mkdir -p bin obj/pdf
	cd obj/pdf && texi2pdf ../../"$<" < /dev/null
	mv obj/pdf/$*.pdf $@

.PHONY: dvi
dvi: bin/mongoclock.dvi
bin/%.dvi: doc/info/%.texinfo
	@! test -d obj/dvi || rm -rf obj/dvi
	@mkdir -p bin obj/dvi
	cd obj/dvi && $(TEXI2DVI) ../../"$<" < /dev/null
	mv obj/dvi/$*.dvi $@

.PHONY: ps
ps: bin/mongoclock.ps
bin/%.ps: doc/info/%.texinfo
	@! test -d obj/ps || rm -rf obj/ps
	@mkdir -p bin obj/ps
	cd obj/ps && texi2pdf --ps ../../"$<" < /dev/null
	mv obj/ps/$*.ps $@



.PHONY: install
install: install-base install-info install-man

.PHONY: install-all
install-all: install-base install-doc

.PHONY: install-base
install-base: install-cmd install-license

.PHONY: install-command
install-command: bin/mongoclock
	install -dm755   -- "$(DESTDIR)$(BINDIR)"
	install -m755 $< -- "$(DESTDIR)$(BINDIR)/$(COMMAND)"

.PHONY: install-license
install-license:
	install -dm755        -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"
	install -m644 COPYING -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"

.PHONY: install-doc
install-doc: install-info install-pdf install-dvi install-ps install-man

.PHONY: install-info
install-info: bin/mongoclock.info
	install -dm755   -- "$(DESTDIR)$(INFODIR)"
	install -m644 $< -- "$(DESTDIR)$(INFODIR)/$(PKGNAME).info"

.PHONY: install-pdf
install-pdf: bin/mongoclock.pdf
	install -dm755   -- "$(DESTDIR)$(DOCDIR)"
	install -m644 $< -- "$(DESTDIR)$(DOCDIR)/$(PKGNAME).pdf"

.PHONY: install-dvi
install-dvi: bin/mongoclock.dvi
	install -dm755   -- "$(DESTDIR)$(DOCDIR)"
	install -m644 $< -- "$(DESTDIR)$(DOCDIR)/$(PKGNAME).dvi"

.PHONY: install-ps
install-ps: bin/mongoclock.ps
	install -dm755   -- "$(DESTDIR)$(DOCDIR)"
	install -m644 $< -- "$(DESTDIR)$(DOCDIR)/$(PKGNAME).ps"

.PHONY: install-man
install-man: doc/man/mongoclock.1
	install -dm755   -- "$(DESTDIR)$(MAN1DIR)"
	install -m644 $< -- "$(DESTDIR)$(MAN1DIR)/$(COMMAND).1"



.PHONY: uninstall
uninstall:
	-rm    -- "$(DESTDIR)$(BINDIR)/$(COMMAND)"
	-rm    -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)/COPYING"
	-rmdir -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"
	-rm    -- "$(DESTDIR)$(INFODIR)/$(PKGNAME).info"
	-rm    -- "$(DESTDIR)$(DOCDIR)/$(PKGNAME).pdf"
	-rm    -- "$(DESTDIR)$(DOCDIR)/$(PKGNAME).dvi"
	-rm    -- "$(DESTDIR)$(DOCDIR)/$(PKGNAME).ps"
	-rm    -- "$(DESTDIR)$(MAN1DIR)/$(COMMAND).1"



.PHONY: clean
clean:
	-rm -f bin obj


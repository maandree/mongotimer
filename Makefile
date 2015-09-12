# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

PREFIX = /usr
DATA = /share
BIN = /bin
PKGNAME = mongoclock
COMMAND = mongoclock
LICENSES = $(PREFIX)$(DATA)


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


all: mongoclock doc

doc: info

info: mongoclock.info

%.info: info/%.texinfo
	makeinfo "$<"

mongoclock: bin/mongoclock

bin/mongoclock: obj/mongoclock.o
	mkdir -p bin
	$(CC) $(FLAGS) -o $@ $^ $(LDFLAGS)

obj/mongoclock.o: obj/mongoclock.c
	$(CC) $(FLAGS) -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

obj/mongoclock.c: src/mongoclock.c
	mkdir -p obj
	$(GPP) -s '$$' -i $< -o $@

install: install-cmd install-license install-info

install-cmd: bin/mongoclock
	install -dm755 "$(DESTDIR)$(PREFIX)$(BIN)"
	install -dm755 "$(DESTDIR)$(PREFIX)$(LIBEXEC)"
	install -m755 bin/mongoclock "$(DESTDIR)$(PREFIX)$(BIN)/$(COMMAND)"

install-license:
	install -dm755 "$(DESTDIR)$(LICENSES)/$(PKGNAME)"
	install -m644 COPYING "$(DESTDIR)$(LICENSES)/$(PKGNAME)"

install-info: mongoclock.info
	install -dm755 "$(DESTDIR)$(PREFIX)$(DATA)/info"
	install -m644 mongoclock.info "$(DESTDIR)$(PREFIX)$(DATA)/info/$(PKGNAME).info"

uninstall:
	-rm -- "$(DESTDIR)$(PREFIX)$(BIN)/$(COMMAND)"
	-rm -- "$(DESTDIR)$(LICENSES)/$(PKGNAME)/COPYING"
	-rmdir -- "$(DESTDIR)$(LICENSES)/$(PKGNAME)"
	-rm -- "$(DESTDIR)$(PREFIX)$(DATA)/info/$(PKGNAME).info"

.PHONY: clean
clean:
	-rm -f bin obj mongoclock.info


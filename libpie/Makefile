# Builds libpie.a, the Raspberry Pi Sense HAT emulator static library.

CC ?= gcc
AR ?= ar
CFLAGS ?= -O2 -Wall -Wextra

# Installs into $(PREFIX) instead of /usr/local so no root access is needed.
# Add $(PREFIX)/include to CPATH and $(PREFIX)/lib to LIBRARY_PATH to link
# with plain -lpie. To install system-wide, run: make install PREFIX=/usr/local
PREFIX ?= $(HOME)/.local

libpie.a: piemulator.o
	$(AR) rcs $@ $^

piemulator.o: piemulator.c piemulator.h
	$(CC) $(CFLAGS) -c piemulator.c -o $@

install: libpie.a
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	cp libpie.a $(PREFIX)/lib/
	cp piemulator.h $(PREFIX)/include/

uninstall:
	rm -f $(PREFIX)/lib/libpie.a
	rm -f $(PREFIX)/include/piemulator.h

clean:
	rm -f piemulator.o libpie.a

.PHONY: install uninstall clean

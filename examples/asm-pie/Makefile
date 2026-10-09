# Builds the aarch64 assembly examples against the installed libpie.a,
# exactly like any other program that uses the library.
# The library must be installed for aarch64 first:
#   make -C ../../libpie install CC=aarch64-linux-gnu-gcc   (cross compiling)
#   make -C ../../libpie install                            (on a Raspberry Pi)
# Set CC=gcc to build natively on a Raspberry Pi.

CC ?= aarch64-linux-gnu-gcc
CFLAGS ?= -O2 -Wall

EXAMPLES := dan towerex1 towerex2

.PHONY: all clean

all: $(EXAMPLES)

sleepy.o: sleepy.s
	$(CC) -c $< -o $@

dan: dan.s sleepy.o
	$(CC) -static -o $@ $< sleepy.o -lpie -pthread

towerex1: towerex1.s sleepy.o
	$(CC) -static -o $@ $< sleepy.o -lpie -pthread

towerex2: towerex2.s sleepy.o
	$(CC) -static -o $@ $< sleepy.o -lpie -pthread

clean:
	rm -f *.o $(EXAMPLES)
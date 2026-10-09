# Builds every example program in this directory against the installed
# libpie.a, exactly like any other program that uses the library.
# Install the library first (see the top-level README), then run make.
# Binaries are written to bin/.

CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra
LIBS := -lpie -pthread
BIN_DIR := bin

PROGRAMS := brb coord-test gpt joytest d2 dmihovch scroll cisc210-scroll seg towerc

.PHONY: all clean $(PROGRAMS)

all: $(PROGRAMS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

brb: brb/brb.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

coord-test: coord-test/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

gpt: gpt/chat-gpt-test.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS) -lm

joytest: joytest/joytest.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

d2: monogram/d2.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

dmihovch: monogram/dmihovch.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

scroll: scroll/main.c scroll/display.c scroll/input.c scroll/scroll.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ scroll/main.c scroll/display.c scroll/input.c $(LIBS) -lm

cisc210-scroll: cisc210-scroll/cisc210-scroll-main/main.c cisc210-scroll/cisc210-scroll-main/display.c cisc210-scroll/cisc210-scroll-main/input.c cisc210-scroll/cisc210-scroll-main/scroll.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ cisc210-scroll/cisc210-scroll-main/main.c cisc210-scroll/cisc210-scroll-main/display.c cisc210-scroll/cisc210-scroll-main/input.c $(LIBS) -lm

seg: seg/fault.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

towerc: towerc/tower.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LIBS)

clean:
	rm -rf $(BIN_DIR)
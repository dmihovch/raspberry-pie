#!/usr/bin/env  bash
#
#
#
# to use: ./build [path to program]
#
#
#
#
#
#



PROGRAM=$@

gcc -o bin/pie $PROGRAM src/piscreen.c -lncurses -pthread -lm

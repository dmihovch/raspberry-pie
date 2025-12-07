# Raspberry-Pi-Emulator
Raspberry Pi Sense Hat Emulator

Raspberry Pie is a library for programming the emulated sensehat
of a raspberry pi. The library is built to work with the Libsense library
we use here at UD, with identical function signatures.
This means that users can link their programs against this library to
test and develop without the hassle of the actual hardware, and then link
the same program against the Libsense library in order to test and develop
on the pi itself. At the moment, I do believe the library is POSIX compliant, 
but sadly(?) I haven't figured out (and probably won't) Windows compatibility. If
anybody would like to port the library to Windows, go right ahead.


This project is a work in progress, and has some limitations that
(potentially) won't be able to be circumvented. This library should only
be used for quick testing or on the go development.


TODO:

1. Figure out if it is pixel[y][x] or pixel[x][y]
2. Simulate segfault on out-of-bounds pixel writes(?(idk if this will be possible))
3. Make a build script that isn't terrible, compiling these programs is annoying
4. Do some real, honest to god testing
5. Then do some real, honest to god stress testing

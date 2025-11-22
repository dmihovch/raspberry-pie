# raspberry-pie
Raspberry Pi Sense Hat Emulator

Raspberry Pie is a library for programming the emulated sensehat
of a raspberry pi. The library is built to work with Professor
Roosen of UD's Libsense library, with identical function signatures.
This means that users can link their programs against this library to
test and develop without the hassle of the actual hardware, and then link
the same program against the Libsense library in order to test and develop
on the pi itself. The only two dependecies outside the C standard library
are pthreads and ncurses.

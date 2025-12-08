# Raspberry-Pi-Emulator
Raspberry Pi Sense Hat Emulator

Raspberry Pie is a library for programming the emulated sensehat
of a raspberry pi. The library is built to work with the Libsense library
we use here at UD, with identical function signatures.
This means that users can link their programs against this library to
test and develop without the hassle of the actual hardware, and then link
the same program against the Libsense library in order to test and develop
on the pi itself. I do believe the library is POSIX compliant, 
but sadly(?) I haven't figured out (and probably won't) Windows compatibility. If
anybody would like to port the library to Windows, go right ahead.


This project is a work in progress, and has some limitations that
(potentially) won't be able to be circumvented. This library should only
be used for quick testing or on the go development.








USAGE:

To use Raspberry PiE, git clone this repo, and run the build script ?


To compile and link against code manually 

Ensure that your project, sense.h piemulator.c, and piemulator.h are in the same directory.

Switch each '#include "sense.h"' -> '#include "piemulator.h"'


gcc -c piemulator.c -pthread -o piemulator.o
gcc -c example.c -o example.o
gcc -o example example.o piemulator.o




To install as a static library (gcc -o example example.c -lpie)

To link against your code, make sure that 'piemulator.h' is in the same 
directory as 'sense.h', and then simply replace all of your 
'#include "sense.h"' with '#include "piemulator.h" 
and then instead of compiling with '-lsense', compile with '-lpie' 



the arrow keys work as the joystick, and enter/return works as the joystick button


There are some functional limitations to the library. Because of the nature of how
we assign color values to pixels (directly assigning, ie bitmap[x][y] = color), I 
have not yet found a good way to intercept and check the coordinates of the pixel 
the user assigned a value to. On the actual hardware, I believe assigning a value
to a pixel out of bounds it causes a seg fault. In this library, it
currently just doesn't draw anything to the screen. So use this library at your own risk, and
certainly do not solely use this and just pray it works on the actual hardware.



TODO:

1. Figure out if it is pixel[y][x] or pixel[x][y]
2. Simulate segfault on out-of-bounds pixel writes(?(idk if this will be possible))
3. Make a build script that isn't terrible, compiling these programs is annoying
4. Do some real, honest to god testing
5. Then do some real, honest to god stress testing

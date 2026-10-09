# Raspberry Pi Sense HAT Emulator

libpie is a static library that emulates the Raspberry Pi Sense HAT LED
array and joystick in a terminal. It exposes the same function signatures
as libsense, so a program written for the Sense HAT compiles and runs
against either library.

## Supported hardware

- LED array (8 by 8 pixels)
- Joystick

## Not supported

- Gyroscope
- Accelerometer
- Magnetometer
- Temperature and humidity sensor
- Pressure sensor

Programs that call libsense functions for these sensors do not compile
against libpie.

## Dependencies

Build:

- gcc (or another C compiler)
- ar (binutils)
- make

Runtime:

- Linux or another POSIX system. Windows is not supported.
- A terminal with ANSI escape sequence support, including 24-bit color
  (truecolor). Terminals without 24-bit color render colors incorrectly.
- A terminal at least 33 columns by 17 rows for the LED grid. The rotation
  menu and port markers are skipped when the terminal is too small.

Linking:

- Programs link with `-lpie -pthread`. The `-pthread` flag is required
  because the library uses threads internally.

Assembly examples:

- `aarch64-linux-gnu-gcc` to cross compile, or `gcc` on a Raspberry Pi.

## Build and install

Build:

    make -C libpie

This produces `libpie.a`, a static archive containing the C API and the
assembly API.

Install to `$HOME/.local` (no root access needed):

    make -C libpie install

This copies `libpie.a` to `$HOME/.local/lib` and `piemulator.h` to
`$HOME/.local/include`. Linking with `-lpie` then requires:

    CPATH=$HOME/.local/include
    LIBRARY_PATH=$HOME/.local/lib

Install system-wide:

    make -C libpie install PREFIX=/usr/local

A system-wide install needs no `CPATH` or `LIBRARY_PATH` because gcc
searches `/usr/local/include` and `/usr/local/lib` by default.

Uninstall:

    make -C libpie uninstall

## Usage

Replace `#include "sense.h"` with `#include <piemulator.h>` and link with
`-lpie` instead of `-lsense`:

    gcc -o example example.c -lpie -pthread

C API:

- `getFrameBuffer`
- `freeFrameBuffer`
- `clearFrameBuffer`
- `getColor`
- `getJoystickDevice`
- `freeJoystick`
- `pollJoystick`

Assembly API (aarch64):

- `openfb`
- `closefb`
- `setPixel`
- `openJoystick`
- `closeJoystick`
- `getJoystickValue`
- `getColor`

## Orientation

The emulator shows the Pi with its USB ports up by default. In this
orientation `pixel[0][0]` is the bottom-left LED, x increases to the
right, and y increases upward.

Press R to rotate the Pi 90 degrees clockwise. USB and ETH markers on the
edges of the grid show the port positions for the current orientation. A
menu in the top-left corner shows the R key binding.

The arrow keys map to the on-screen directions for the current
orientation. Enter or return is the joystick button. The R key is consumed
by the emulator and never delivered to the user program.

## Examples

Install the library first, then:

    make -C examples/c-pie
    make -C examples/asm-pie

The C examples are written to `examples/c-pie/bin/`. The assembly examples
require the library installed for aarch64:

    make -C libpie install CC=aarch64-linux-gnu-gcc   # cross compiling
    make -C libpie install                            # on a Raspberry Pi

## Caveats

- The emulator takes over the terminal while a framebuffer is open. It
  clears the screen and hides the cursor. It restores the terminal on
  exit, including on SIGINT, SIGQUIT, SIGTERM, and SIGSEGV.
- The joystick owns stdin. A program that calls `getJoystickDevice` cannot
  also read stdin itself. A program that reads stdin itself must not open
  the joystick.
- The joystick puts stdin in raw mode. Framebuffer-only programs leave the
  terminal in canonical mode, so `scanf` works interactively. Typed input
  echoes on the line below the grid when the terminal has room for it.
- Piped stdin works for framebuffer-only programs. The joystick thread
  also reads piped stdin; raw mode is skipped because a pipe is not a
  terminal.
- The palette reset escape sequence (`\033]104`) is xterm-specific. Other
  terminals ignore it.
- The library is single-instance. One framebuffer and one joystick can be
  open at a time.

## Limitations

- Out-of-bounds pixel writes are not detected or prevented. The real
  hardware faults on out-of-bounds writes.
- `pollJoystick` ignores its timeout argument and returns immediately.
- Colors round-trip through RGB565, which loses precision compared to
  24-bit color.
- The refresh loop sleeps 16667 microseconds per frame.
- The framebuffer is not double buffered.
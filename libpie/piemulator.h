#ifndef PIEMULATOR_H
#define PIEMULATOR_H

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/*
 * Libsense Wrapper Definitions
 */

//Framebuffer Wrapper Definitions

typedef struct {
    char id[16];
    char padding[256];
} fb_fix_screeninfo;

typedef struct {
	uint16_t pixel[8][8];
} sense_fb_bitmap_t;

typedef struct {
    int fd;
	fb_fix_screeninfo info;
	sense_fb_bitmap_t* bitmap;
} pi_framebuffer_t;

pi_framebuffer_t* getFrameBuffer(void);
void freeFrameBuffer(pi_framebuffer_t* device);
void clearFrameBuffer(pi_framebuffer_t* fb, uint16_t color);
uint16_t getColor(int red, int green, int blue);


//Joystick Wrapper Definitions

//Key codes reported by the physical Sense HAT joystick (see linux/input.h).
#define KEY_ENTER 28
#define KEY_UP 103
#define KEY_DOWN 108
#define KEY_RIGHT 106
#define KEY_LEFT 105

typedef struct {
	int _fd;
	char _name[256];
} pi_joystick_t;

pi_joystick_t* getJoystickDevice(void);
void freeJoystick(pi_joystick_t* device);
void pollJoystick(pi_joystick_t* device, void (*callback)(unsigned int code), int timeout);


/*
 * Emulator Definitions
 */

//Joystick Emulator Definitions

typedef struct {
	int keyCode;
	int read;
	pthread_mutex_t lock;
} JoystickPipeline;

int PieInitJoystick(void);
void PieCloseJoystick(void);
int PieGetJoystickValue(void);
void* PieJoystickThread(void* payload);


//Framebuffer Emulator Definitions

typedef struct {
    int x;
    int y;
    uint16_t color565;
} EmulatedPixel;

typedef struct {
    EmulatedPixel pixels[8][8];
    sense_fb_bitmap_t* userFb;
    pthread_t refreshThread;
    int killThread;
    JoystickPipeline joystickPipe;
    pthread_t joystickPollingThread;
    int killJoystickThread;
    int rotation; // 0 = USB ports up; each R press rotates the Pi 90 degrees clockwise
} PieState;

void PieSetPixel(int x, int y, uint16_t color565);
int PieInitFrameBuffer(void);
int PieInitGraphic(void);
int PieCloseGraphic(void);
void* PieRefreshThread(void* payload);
void PieUserFBtoState(sense_fb_bitmap_t* userFb);
void PieCleanExit(int sig);

uint16_t RGB255toRGB565(int r, int g, int b);

void CursorMove(int y, int x);
void PiePrintChar(int y, int x, char c);
void DisableRawMode(void);
void EnableRawMode(void);
void HandleResize(int sig);
void PieHandleSegFault(int sig);
void PieRedrawGraphic(void);


//Debug/Util Functions

void PieDebug(void);


/*
 * Assembly API
 *
 * Flat C functions callable from aarch64 assembly programs. See
 * examples/asm-pie/ for examples.
 */

int openfb(void);
int closefb(void);
void setPixel(int x, int y, uint16_t color);
int openJoystick(void);
void closeJoystick(void);
int getJoystickValue(void);

#endif

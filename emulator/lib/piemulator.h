
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
/*
 *
 *
 *
 *
 *  Libsense Wrapper Definitions
 *
 *
 *
 *
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



pi_framebuffer_t* getFrameBuffer();
void freeFrameBuffer(pi_framebuffer_t* device);
void clearFrameBuffer(pi_framebuffer_t* fb,uint16_t color);
uint16_t getColor(int red,int green,int blue);




//Joystick Wrapper Definitions

#define KEY_ENTER 28
#define KEY_UP 103
#define KEY_DOWN 108
#define KEY_RIGHT 106
#define KEY_LEFT 105

typedef struct {
	int _fd;
	char _name[256];
} pi_joystick_t;

pi_joystick_t* getJoystickDevice();
void freeJoystick(pi_joystick_t* device);
void pollJoystick(pi_joystick_t* device, void (*callback)(unsigned int code),int timeout);









/*
 *
 *
 *
 *
 * Emulator Definitions
 *
 *
 *
 *
 *
 */




//Joystick Emulator Definitions




typedef struct {
	int keyCode;
	int read;
	pthread_mutex_t lock;
} JoystickPipeline;

int PieInitJoystick();
void PieCloseJoystick();
int PieGetJoystickValue();
void* PieJoystickThread(void*);



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
} PieState;

void PieSetPixel(int, int, uint16_t);
int PieInitFrameBuffer();
int PieInitGraphic();
int PieCloseGraphic();
void* PieRefreshThread(void*);
void PieUserFBtoState(sense_fb_bitmap_t*);
void PieCleanExit(int);



uint16_t RGB255toRGB565(int,int,int);


void CursorMove(int,int);
void PiePrintChar(int,int,char);
void DisableRawMode();
void EnableRawMode();
void HandleResize(int);
void PieHandleSegFault();
void PieRedrawGraphic();



/*
 *
 * Libsense-asm Wrapper Definitions
 *
 *
 */


 int openfb();
 int closefb();
 void setPixel(int ,int , uint16_t);
 int openJoystick();
 void closeJoystick();
 int getJoystickValue();


//Debug/Util Functions

void PieDebug();

void sleepy();

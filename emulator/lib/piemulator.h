
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>



/*
 *
 *
 *
 *
 *  Libsense Wrapper Functions and Structs
 *
 *
 *
 *
*/


//Framebuffer Wrappers and Structs

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




//Joystick Wrappers and Structs

//these need to exist for the user, it is the joystick codes
//The issue is that ncurses uses the same names for these Macros, but with different values
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
 * Emulator Functions and Structs
 *
 *
 *
 *
 *
 */




//Joystick Functions and Structs




typedef struct {
	int keyCode;
	int read;
	pthread_mutex_t lock;
} JoystickPipeline;

int PieInitJoystick();
void PieCloseJoystick();
int PieGetJoystickValue();
void* PieJoystickThread(void*);



//Framebuffer Functions and Structs

typedef struct {
    int r;
    int g;
    int b;
    int saved;
} SavedColor;


typedef struct {
    int x;
    int y;
    uint16_t color565;
} EmulatedPixel;

typedef struct {
    EmulatedPixel pixels[8][8];
    sense_fb_bitmap_t* userFb;
    uint16_t colorCache[256];
    int nextColorIdx;
    pthread_t refreshThread;
    int killThread;
    SavedColor origColors[256];
    int maxColors;
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
uint16_t RGB255toRGB565(int,int,int);
void SaveOriginalColor(int);
void RestoreOriginalColors();
int FindCloseColorId(uint16_t);



//Debug/Util Functions

void PieDebug();

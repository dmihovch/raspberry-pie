
#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>



//Libsense Wrapper Functions and Structs
typedef struct {
    char _id[16];
    char _padding[256];
} fb_fix_screeninfo;

typedef struct {
	uint16_t pixel[8][8];
} sense_fb_bitmap_t;

typedef struct {
    int _fd;
	fb_fix_screeninfo _info;
	sense_fb_bitmap_t* bitmap;
} pi_framebuffer_t;



pi_framebuffer_t* getFrameBuffer();
void freeFrameBuffer(pi_framebuffer_t* device);
void clearFrameBuffer(pi_framebuffer_t* fb,uint16_t color);
uint16_t getColor(int red,int green,int blue);


//Emulator Functions and Structs

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
} State;

void PieSetPixel(int, int, uint16_t);
int InitPieFrameBuffer();
int DrawPie();
int InitPieGraphic();
int ClosePieGraphic();
void* PieRefreshThread(void*);
void SendUserFBtoGlobalState(sense_fb_bitmap_t*);
uint16_t RGB255toRGB565(int,int,int);
void SaveOriginalColor(int);
void RestoreOriginalColors();
void PieDebug();

//need to implement
int FindCloseColorId(uint16_t);

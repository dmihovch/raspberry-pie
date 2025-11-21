
#pragma once

#include "picolors.h"
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#define CURSES_WHITE 7
#include "../../func-wrappers-c/include/framebuffer-pie.h"



typedef struct {
    int x;
    int y;
    ColorRaw color;
} EmulatedPixel;

typedef struct {
    EmulatedPixel pixels[8][8];
    ColorCacheEntry* colorsCache[256];
    int nextCursesColorIdx;
    pi_framebuffer_t* userFB;
    pthread_t refreshThread;
    int killThread;
} FrameBuffer;

/*
 * x
 * y
 * cursesColorIdx
 */
void PieSetPixel(int, int, uint16_t);
int InitPieFrameBuffer();
int DrawPie();
int InitPieGraphic();
int ClosePieGraphic();
int AddColorToCache(ColorRaw);
int FindCloseColor(ColorRaw);
int HandleGetColor(ColorRaw);
int ColorHash(uint16_t);
void* PieRefreshThread(void*);
void SendUserFBtoGlobalState(pi_framebuffer_t*);
//util functions
void PieDebug();

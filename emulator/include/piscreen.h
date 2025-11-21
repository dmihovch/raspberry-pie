
#pragma once

#include "picolors.h"
#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>




typedef struct {
    int x;
    int y;
    ColorRaw color;
} EmulatedPixel;

typedef struct {
    EmulatedPixel pixels[8][8];
    ColorCacheEntry* colorsCache[256];
    int nextCursesColorIdx;
} FrameBuffer;

/*
 * x
 * y
 * r
 * g
 * b
 */
int PieSetPixel(int, int, ColorRaw);
int InitPieFrameBuffer();
int DrawPie();
int InitPieGraphic();
int ClosePieGraphic();
int AddColorToCache(ColorRaw);
ColorRaw FindCloseColor(ColorRaw);
int HandleGetColor(ColorRaw);
int ColorHash(uint16_t);
//util functions
void PieDebug();

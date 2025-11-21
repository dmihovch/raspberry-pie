
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
    int nextColorIdx;
} FrameBuffer;

/*
 * x
 * y
 * r
 * g
 * b
 */
int PieSetPixel(int, int, int, int, int);
int InitPieFrameBuffer();
int DrawPie();
int InitPieGraphic();
int ClosePieGraphic();

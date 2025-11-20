#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct ColorCacheEntry{
	uint16_t color;
	int cursesColorIdx;
	struct ColorCacheEntry* next;
}ColorCacheEntry;
typedef struct {
    int x;
    int y;
    uint16_t color;
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
void PieSetPixel(FrameBuffer*, int, int, int, int, int);
FrameBuffer* InitPieFrameBuffer();
void DrawPie(FrameBuffer*);
void InitPieGraphic(FrameBuffer*);
void ClosePieGraphic();
uint16_t PieCreateColor(int,int,int);

#pragma once
#include <stdint.h>
#include <stdlib.h>
typedef enum {RGB255=0, RGB565, CURSES} ColorType;

//r,g,b will be scaled to ncurses 0-1000 values
typedef struct {
	int r;
	int g;
	int b;
	uint16_t rgb565;
	ColorType type;
} ColorRaw;


typedef struct ColorCacheEntry{
	ColorRaw color; //probably RGB565?
	int cursesColorIdx;
	struct ColorCacheEntry* next;
}ColorCacheEntry;

// uint16_t PieCreateColor(int,int,int);
uint16_t RGB255toRGB565(int,int,int);
ColorRaw CreateColorRaw(int,int,int);
ColorCacheEntry* CreateColorCacheEntry(ColorRaw, int);

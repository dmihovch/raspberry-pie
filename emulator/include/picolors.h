#pragma once
#include <stdint.h>

typedef enum {RGB255=0, RGB565, CURSES} ColorType;

typedef struct {
	int r;
	int g;
	int b;
	ColorType type;
} ColorRaw;


typedef struct ColorCacheEntry{
	uint16_t color; //probably RGB565?
	int cursesColorIdx;
	struct ColorCacheEntry* next;
}ColorCacheEntry;
uint16_t PieCreateColor(int,int,int);

uint16_t RGB255toRGB565(int,int,int);
ColorRaw ConvertColorTo(ColorRaw, ColorType);

#pragma once
#include <stdint.h>

typedef struct {
    char id[16];
    char _padding[256];
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
void clearFrameBuffer(pi_framebuffer_t* bitmap,uint16_t color);
uint16_t getColor(int red,int green,int blue);

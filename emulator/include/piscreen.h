#include <ncurses.h>
#include <stdlib.h>

typedef struct {
    uint16_t pixels[8][8];
} FrameBuffer;


/*
 * x
 * y
 * r
 * g
 * b
 */
void PieSetPixel(int, int, int, int, int);
FrameBuffer* InitPieFrameBuffer();
void DrawPie(FrameBuffer*);
void InitPieGraphic();
void ClosePieGraphic();

#include "../include/piscreen.h"

void PieSetPixel(int x, int y, int r, int g, int b){

}

FrameBuffer* InitPieFrameBuffer(){

    FrameBuffer* fb = calloc(1,sizeof(FrameBuffer));
    if(fb == NULL) return NULL;
    return fb;



    InitPieGraphic();

}

void InitPieGraphic(){

    initscr();




}

void DrawPie(FrameBuffer* fb){




}

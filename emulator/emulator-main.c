#include "include/piscreen.h"

//for testing
int main(){

    FrameBuffer* fb = InitPieFrameBuffer();
    if(fb == NULL) return 1;
    getch();


    PieSetPixel(fb, 4, 5, 240, 50, 255);

    getch();
    ClosePieGraphic();

    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            printf("x:%d y:%d\n",fb->pixels[i][j].x,fb->pixels[i][j].y);
        }
    }

    free(fb);
    return 0;

}

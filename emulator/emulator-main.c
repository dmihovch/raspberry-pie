#include "include/piscreen.h"

//for testing
int main(){

    if(InitPieFrameBuffer())return 1;
    getch();


    PieSetPixel(4, 5, 240, 50, 255);

    getch();
    ClosePieGraphic();



    return 0;

}

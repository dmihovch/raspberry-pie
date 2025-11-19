#include "../include/piscreen.h"

int Xmax;
int Ymax;

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

    Xmax = getmaxx(stdscr);
    Ymax = getmaxy(stdscr);

    int lineHeight = 17;
    int lineWidth = 33;
    int start_y = (Ymax - lineHeight)/2;
    int start_x = (Xmax - lineWidth)/2;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            mvaddch(start_y + y * 2, start_x + x * 4, '+');
            mvaddch(start_y + y * 2, start_x + x * 4 + 1, '-');
            mvaddch(start_y + y * 2, start_x + x * 4 + 2, '-');
            mvaddch(start_y + y * 2, start_x + x * 4 + 3, '-');
        }
        mvaddch(start_y + y * 2, start_x + 32, '+');

        for (int x = 0; x < 8; x++) {
            mvaddch(start_y + y * 2 + 1, start_x + x * 4, '|');
            mvaddch(start_y + y * 2 + 1, start_x + x * 4 + 1, ' ');
            mvaddch(start_y + y * 2 + 1, start_x + x * 4 + 2, '*');
            mvaddch(start_y + y * 2 + 1, start_x + x * 4 + 3, ' ');
        }
        mvaddch(start_y + y * 2 + 1, start_x + 32, '|');
    }

    for (int x = 0; x < 8; x++) {
        mvaddch(start_y + 16, start_x + x * 4, '+');
        mvaddch(start_y + 16, start_x + x * 4 + 1, '-');
        mvaddch(start_y + 16, start_x + x * 4 + 2, '-');
        mvaddch(start_y + 16, start_x + x * 4 + 3, '-');
    }
    mvaddch(start_y + 16, start_x + 32, '+');

    refresh();



}

void ClosePieGraphic(){

    endwin();

}

void DrawPie(FrameBuffer* fb){




}

#include "../include/piscreen.h"





void PieSetPixel(FrameBuffer* fb, int x, int y, int r, int g, int b){


	//?
    int rScaled, gScaled, bScaled, xGrid, yGrid;
    rScaled = (r * 1000) / 255;
    gScaled = (g * 1000) / 255;
    bScaled = (b * 1000) / 255;

    xGrid = fb->pixels[x][y].x;
    yGrid = fb->pixels[x][y].y;



    //I'm thinking this is going to be incorrect once I implement the color hashing
    //Unless it becomes a performance issue, I'm just going to have the colors overwrite eachother
    init_color(fb->nextColorIdx, rScaled, gScaled, bScaled);
    init_pair(fb->nextColorIdx, fb->nextColorIdx, COLOR_BLACK);
    attron(COLOR_PAIR(fb->nextColorIdx));
    mvaddch(yGrid,xGrid,'*');
    attroff(COLOR_PAIR(fb->nextColorIdx));
    refresh();



}

FrameBuffer* InitPieFrameBuffer(){

    initscr();
    noecho();
    curs_set(0);

    if(!has_colors()){
        return NULL;
    }
    start_color();





    FrameBuffer* fb = calloc(1,sizeof(FrameBuffer));
    if(fb == NULL) return NULL;

    fb->nextColorIdx = 8;
    InitPieGraphic(fb);

    return fb;
}

void InitPieGraphic(FrameBuffer* fb){


    int Xmax = getmaxx(stdscr);
    int Ymax = getmaxy(stdscr);

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
            mvaddch(start_y + y * 2 + 1, start_x + x * 4 + 2, ' ');
            fb->pixels[x][y].x = (start_x+x*4+2);
            fb->pixels[x][y].y = (start_y+y*2+1);
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

	//some of these calls are probably redundant
    endwin();
    reset_shell_mode();
    reset_color_pairs();
    use_default_colors();

}

void DrawPie(FrameBuffer* fb){




}

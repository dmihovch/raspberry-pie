#include "../include/piscreen.h"

static FrameBuffer fb;


int PieSetPixel(int x, int y, ColorRaw color){


	//?
    int rScaled = color.r;
    int gScaled = color.g;
    int bScaled = color.b;

    int xGrid = fb.pixels[x][y].x;
    int yGrid = fb.pixels[x][y].y;



    //I'm thinking this is going to be incorrect once I implement the color hashing
    //Unless it becomes a performance issue, I'm just going to have the colors overwrite eachother
    init_color(fb.nextCursesColorIdx, rScaled, gScaled, bScaled);
    init_pair(fb.nextCursesColorIdx, fb.nextCursesColorIdx, COLOR_BLACK);
    attron(COLOR_PAIR(fb.nextCursesColorIdx));
    mvaddch(yGrid,xGrid,'*');
    attroff(COLOR_PAIR(fb.nextCursesColorIdx));
    refresh();

    return 0;

}

int InitPieFrameBuffer(){

    initscr();
    noecho();
    curs_set(0);

    if(!has_colors()){
        return -1;
    }
    start_color();






    for(int i = 0; i<256; i++) fb.colorsCache[i] = NULL;

    fb.nextCursesColorIdx = 8;
    InitPieGraphic();

    return 0;
}

int HandleGetColor(ColorRaw color){

    //NEED TO HANDLE THIS!!!!
    if(fb.nextCursesColorIdx > 255){
        FindCloseColor(color);


        return -1;
    }
    return AddColorToCache(color);
}

int InitPieGraphic(){


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
            fb.pixels[x][y].x = (start_x+x*4+2);
            fb.pixels[x][y].y = (start_y+y*2+1);
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

    return 0;

}

int ClosePieGraphic(){

    ColorCacheEntry* cur;
    ColorCacheEntry* next;
    for(int i = 0; fb.colorsCache[i]; i++){
        cur = fb.colorsCache[i];
        while(cur){
            next = cur->next;
            free(cur);
            cur = next;
        }

    }

	//some of these calls are probably redundant
	use_default_colors();
    endwin();
    reset_shell_mode();
    //reset_color_pairs();

    return 0;
}

int DrawPie(){



    return 0;
}


int AddColorToCache(ColorRaw color){
    int hash = ColorHash(color.rgb565);
    if(fb.colorsCache[hash] == NULL){
        fb.colorsCache[hash] = CreateColorCacheEntry(color, fb.nextCursesColorIdx);
        if(fb.colorsCache[hash] == NULL){ return -1;}
        fb.nextCursesColorIdx++;
        return 0;
    }

    ColorCacheEntry* cur = fb.colorsCache[hash];
    while(cur->next){
        cur = cur->next;
    }
    cur->next = CreateColorCacheEntry(color, fb.nextCursesColorIdx);
    if(cur->next == NULL){
        return -1;
    }
    fb.nextCursesColorIdx++;
    return 0;
}

ColorRaw FindCloseColor(ColorRaw color){
    return color;
}


int GetNcursesColorID(ColorRaw color){
    int hash = ColorHash(color.rgb565);

    ColorCacheEntry* cur = fb.colorsCache[hash];
    while(cur){
        if(cur->color.rgb565 == color.rgb565){
            return cur->cursesColorIdx;
        }
        cur = cur->next;
    }

    //gonna have to address this
    return 8;

}

int ColorHash(uint16_t color){
    return color%256;
}

void PieDebug(){
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            printf("x:%d y:%d\n",fb.pixels[i][j].x,fb.pixels[i][j].y);
        }
    }
}

#include "../include/piscreen.h"

static FrameBuffer fb;


void PieSetPixel(int x, int y, uint16_t color565){


	//?
    int xGrid = fb.pixels[x][y].x;
    int yGrid = fb.pixels[x][y].y;

    //gonna have to rethink this... again


    init_pair(cursesColorIdx-7, cursesColorIdx, COLOR_BLACK);
    attron(COLOR_PAIR(cursesColorIdx-7));
    mvaddch(yGrid,xGrid,'*');
    attroff(COLOR_PAIR(cursesColorIdx-7));
    refresh();

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
    fb.killThread = 0;
    return InitPieGraphic();

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

    pthread_t refreshThread;
    if(pthread_create(&refreshThread, NULL, PieRefreshThread, NULL) != 0){
        return 1;
    }
    fb.refreshThread = refreshThread;


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
    fb.killThread = 1;
    pthread_join(fb.refreshThread, NULL);

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
        if(fb.colorsCache[hash] == NULL){ return CURSES_WHITE;}
        fb.nextCursesColorIdx++;
        return fb.nextCursesColorIdx -1;
    }

    ColorCacheEntry* cur = fb.colorsCache[hash];
    while(cur->next){
        cur = cur->next;
    }
    cur->next = CreateColorCacheEntry(color, fb.nextCursesColorIdx);
    if(cur->next == NULL){
        return CURSES_WHITE;
    }
    fb.nextCursesColorIdx++;
    return fb.nextCursesColorIdx -1;
}

int FindCloseColor(ColorRaw color){
    return CURSES_WHITE;
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

    if(fb.nextCursesColorIdx < 256){
        return AddColorToCache(color);
    }

    return FindCloseColor(color);

}

int ColorHash(uint16_t color){
    return color%256;
}


void* PieRefreshThread(void* payload){
    int i;
    int j;
    while(!fb.killThread){

        for(i = 0; i<8; i++){
            for(j = 0; j<8; j++){
                PieSetPixel(i,j,fb.userFB->bitmap->pixel[i][j]);
            }
        }


    }

    return NULL;

}

void SendUserFBtoGlobalState(pi_framebuffer_t* userFb){
    fb.userFB = userFb;
}

void PieDebug(){
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            printf("x:%d y:%d\n",fb.pixels[i][j].x,fb.pixels[i][j].y);
        }
    }
}

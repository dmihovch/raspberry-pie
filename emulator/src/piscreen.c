#include "../include/piscreen.h"
#include <ncurses.h>


//Libsense Wrapper Functions

uint16_t getColor(int red, int green, int blue){
    uint16_t color = RGB255toRGB565(red, green, blue);

    return color;
}

pi_framebuffer_t* getFrameBuffer(){

    sense_fb_bitmap_t* bm = calloc(1,sizeof(sense_fb_bitmap_t));
    if(bm == NULL){
        return NULL;
    }

    pi_framebuffer_t* ufb = calloc(1,sizeof(pi_framebuffer_t));
    if(ufb == NULL){
        free(bm);
        return NULL;
    }
    ufb->bitmap = bm;

    SendUserFBtoGlobalState(bm);
    if(InitPieFrameBuffer()!=0){
        free(ufb->bitmap);
        free(ufb);
        return NULL;
    }

    return ufb;

}

void clearFrameBuffer(pi_framebuffer_t* fb,uint16_t color){

	int i,j;
	for(i = 0;i<8;i++){
		for(j=0;j<8;j++){
			fb->bitmap->pixel[i][j] = color;
		}
	}


}

void freeFrameBuffer(pi_framebuffer_t *device){
	free(device->bitmap);
	free(device);
	ClosePieGraphic();
}




//Emulator Functions


static State state;


void PieSetPixel(int x, int y, uint16_t color565){
	int xEmulated = state.pixels[x][y].x;
    int yEmulated = state.pixels[x][y].y;
	if(color565 == 0){
		mvaddch(yEmulated,xEmulated,' ');
		return;
	}




	int colorId = -1;
	int i;
	for(i = 0; i<state.nextColorIdx; i++){
		if(color565 == state.colorCache[i]){
			colorId = i;
			break;
		}
	}

	if(state.nextColorIdx > 255){
		colorId = FindCloseColorId(color565);
	}


	if(colorId == -1 && state.nextColorIdx <= 255){
		state.colorCache[state.nextColorIdx] = color565;


		int r = ((((color565 >> 11) & 0x1F) * 1000 + 15) / 31);
		int g = ((((color565 >> 5) & 0x3F) * 1000 + 31)/ 63);
		int b = (((color565 & 0x1F) * 1000 + 15)/ 31);
		SaveOriginalColor(state.nextColorIdx);
		init_color(state.nextColorIdx, r, g, b);
		init_pair(state.nextColorIdx,state.nextColorIdx,-1);
		colorId = state.nextColorIdx;
		state.nextColorIdx++;
	}

    attron(COLOR_PAIR(colorId));
    mvaddch(yEmulated,xEmulated,'*');
    attroff(COLOR_PAIR(colorId));
}

int InitPieFrameBuffer(){

    initscr();
    noecho();
    curs_set(0);

    if(!has_colors()){
        endwin();
        return -1;
    }


    start_color();

    state.maxColors = COLORS;
    if (state.maxColors > 256) {
        state.maxColors = 256;
    }

    for (int i = 0; i < state.maxColors; i++) {
        state.origColors[i].saved = 0;
    }

    for (int i = 0; i < state.maxColors; i++) {
        SaveOriginalColor(i);
    }
    use_default_colors();
    state.nextColorIdx = 1;
    state.killThread = 0;
    return InitPieGraphic();

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
            state.pixels[x][y].x = (start_x+x*4+2);
            state.pixels[x][y].y = (start_y+y*2+1);
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
    state.refreshThread = refreshThread;


    return 0;

}

int ClosePieGraphic(){

    state.killThread = 1;
    pthread_join(state.refreshThread, NULL);

    if (state.maxColors > 0) {
        for (int i = 0; i < state.maxColors; i++) {
            if (state.origColors[i].saved) {
                init_color(i, state.origColors[i].r, state.origColors[i].g, state.origColors[i].b);
            }
        }
    }

    for (int i = 1; i < state.nextColorIdx && i < 256; i++) {
        init_pair(i, COLOR_WHITE, COLOR_BLACK);
    }

	curs_set(1);
    endwin();
    reset_shell_mode();

    printf("\033[0m\033[?25h");
    printf("\033]104\007");
    fflush(stdout);

    return 0;
}

void* PieRefreshThread(void* payload){
    int i;
    int j;
    while(!state.killThread){

        for(i = 0; i<8; i++){
            for(j = 0; j<8; j++){
                PieSetPixel(i,j,state.userFb->pixel[i][j]);

            }
        }

        refresh();
        usleep(16667);


    }

    return NULL;

}

void SendUserFBtoGlobalState(sense_fb_bitmap_t* userFb){
    state.userFb = userFb;
}



int FindCloseColorId(uint16_t color565){
	return 0;
}



uint16_t RGB255toRGB565(int r,int g,int b){
    //ripped straight from libsense
    r=(float)r / 255.0 * 31.0 + 0.5;
	g=(float)g/ 255.0 * 63.0 + 0.5;
	b=(float)b / 255.0 * 31.0 + 0.5;
	return r<<11|g<<5|b;
}

void SaveOriginalColor(int colorId) {
    if (state.maxColors<=0 || colorId < 0 || colorId >= state.maxColors || state.origColors[colorId].saved) {
        return;
    }

    short r, g, b;
    if (color_content(colorId, &r, &g, &b) == OK) {
        state.origColors[colorId].r = r;
        state.origColors[colorId].g = g;
        state.origColors[colorId].b = b;
        state.origColors[colorId].saved = 1;
    } else {
        // If we can't read it, don't mark it as saved
        // This prevents trying to restore a color we couldn't read
        state.origColors[colorId].saved = 0;
    }
}

void RestoreOriginalColors() {
    // This function might not be needed if we restore in ClosePieGraphic directly
    // But keeping it for consistency
    if (state.maxColors <= 0) {
        return;
    }

    for (int i = 0; i < state.maxColors; i++) {
        if (state.origColors[i].saved) {
            init_color(i, state.origColors[i].r, state.origColors[i].g, state.origColors[i].b);
        }
    }
}

void PieDebug(){
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            printf("x:%d y:%d\n",state.pixels[i][j].x,state.pixels[i][j].y);
        }
    }
}

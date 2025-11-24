#include "piemulator.h"
#include <ncurses.h>
/*
 *
 *
 * Libsense Wrapper Functions
 *
 *
 */

//Framebuffer Wrapper Functions

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


//Joystick Wrapper Functions and Definition

#define JOY_KEY_ENTER 28
#define JOY_KEY_UP 103
#define JOY_KEY_DOWN 108
#define JOY_KEY_RIGHT 106
#define JOY_KEY_LEFT 105

pi_joystick_t* getJoystickDevice(){

	//will probably need more than this
	pi_joystick_t* js =  calloc(1,sizeof(pi_joystick_t));
	if(js == NULL){
		return NULL;
	}

	if(PieInitJoystick()!=0){
		free(js);
		return NULL;
	}

	return js;


}

void freeJoystick(pi_joystick_t *device){
	if(device == NULL) return;
	free(device);
	PieCloseJoystick();
}

void pollJoystick(pi_joystick_t *device, void (*callback)(unsigned int), int timeout){
	int code = PieGetJoystickValue();
	//I don't use the timeout here, that may become a problem
	if(code == -1)return;
	callback(code);
}




/*
 *
 *
 * Emulator Functions and Global State
 *
 *
 */


#define REFRESH60 16667
#define KEY_NEWLINE 10
static State state;



//Framebuffer Wrappers and Definitions

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
    mvaddch(yEmulated,xEmulated,'@');
    attroff(COLOR_PAIR(colorId));
}

int InitPieFrameBuffer(){

    initscr();
    noecho();
    cbreak();
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
            state.pixels[x][y].y = (start_y+(7-y)*2+1);
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

    //escape codes to cleanup and reset the terminal
    printf("\033[0m\033[?25h");
    printf("\033]104\007");
    fflush(stdout);

    return 0;
}

void* PieRefreshThread(void* payload){
	/*
	 *
	 *
	 *	Going to have to ask Roosen about this,
	 *  but I think the grid is supposed to be laid out as
	 *  pixel[y][x] not pixel[x][y]
	 *
	 *
	 */
    int i;
    int j;
    while(!state.killThread){
    	int x,y;
        for(i = 0; i<8; i++){
            for(j = 0; j<8; j++){
                PieSetPixel(i,j,state.userFb->pixel[i][j]);
            }
        }

        refresh();
        usleep(REFRESH60);


    }

    return NULL;

}

void SendUserFBtoGlobalState(sense_fb_bitmap_t* userFb){
    state.userFb = userFb;
}



int FindCloseColorId(uint16_t color565){
	if (state.nextColorIdx == 0) {
        return 0;
    }

    int targetR = (color565 >> 11) & 0x1F;
    int targetG = (color565 >> 5) & 0x3F;
    int targetB = color565 & 0x1F;

    int bestIdx = 0;
    int minDistance = INT_MAX;

    for (int i = 0; i < state.nextColorIdx && i < 256; i++) {
        uint16_t cachedColor = state.colorCache[i];
        int cachedR = (cachedColor >> 11) & 0x1F;
        int cachedG = (cachedColor >> 5) & 0x3F;
        int cachedB = cachedColor & 0x1F;

        int dr = targetR - cachedR;
        int dg = targetG - cachedG;
        int db = targetB - cachedB;
        int distance = (dr * dr) * 4 + (dg * dg) * 2 + (db * db) * 4;

        if (distance < minDistance) {
            minDistance = distance;
            bestIdx = i;
        }
    }

    return bestIdx;
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
        state.origColors[colorId].saved = 0;
    }
}

void RestoreOriginalColors() {
    if (state.maxColors <= 0) {
        return;
    }

    for (int i = 0; i < state.maxColors; i++) {
        if (state.origColors[i].saved) {
            init_color(i, state.origColors[i].r, state.origColors[i].g, state.origColors[i].b);
        }
    }
}



//Joystick Functions

int PieInitJoystick(){
	keypad(stdscr, TRUE);
	pthread_mutex_init(&state.joystickPipe.lock, NULL);
	state.killJoystickThread = 0;
	pthread_t jsThread;
    if(pthread_create(&jsThread, NULL, PieJoystickThread, NULL) != 0){
        return 1;
    }
    state.joystickPollingThread = jsThread;
	return 0;
}


void PieCloseJoystick(){
	state.killJoystickThread = 1;
	pthread_join(state.joystickPollingThread,NULL);
}

int PieGetJoystickValue(){
	int code;
	pthread_mutex_lock(&state.joystickPipe.lock);
	if(state.joystickPipe.read) {
		pthread_mutex_unlock(&state.joystickPipe.lock);
		return -1;
	}
	code = state.joystickPipe.keyCode;
	state.joystickPipe.read = 1;
	pthread_mutex_unlock(&state.joystickPipe.lock);
	return code;
}


void* PieJoystickThread(void* payload){
	int ch,send;
	while(!state.killJoystickThread){
		ch = getch();
		send = 0;
		switch(ch){
			case KEY_UP: {ch = JOY_KEY_UP; send = 1; break;}
			case KEY_DOWN: {ch = JOY_KEY_DOWN; send = 1; break;}
			case KEY_RIGHT: {ch = JOY_KEY_RIGHT; send = 1; break;}
			case KEY_LEFT: {ch = JOY_KEY_LEFT; send = 1; break;}
			case KEY_NEWLINE: {ch = JOY_KEY_ENTER; send = 1; break;}
			case KEY_ENTER: {ch = JOY_KEY_ENTER; send = 1; break;}
		}

		if(send){
			pthread_mutex_lock(&state.joystickPipe.lock);
			state.joystickPipe.keyCode = ch;
			state.joystickPipe.read = 0;
			pthread_mutex_unlock(&state.joystickPipe.lock);
		}

		usleep(REFRESH60);

	}

	return NULL;

}



/*
 *
 *
 * Debug and Util Functions
 *
 *
 *
 */



void PieDebug(){
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            fprintf(stderr,"x:%d y:%d\n",state.pixels[i][j].x,state.pixels[i][j].y);
        }
    }
}

#include "piemulator.h"
#include <stdio.h>




/*
 *
 *
 * Private Definitions
 *
 *
 *
 */

#define REFRESH60 16667
#define KEY_NEWLINE 10



/*
 *
 * Global State
 *
 *
 */
static PieState state;
static struct termios originalTerm;


/*
 *
 *
 * Emulator Functions and Global State
 *
 *
 */




//FrameBuffer Emulator Functions


void PieSetPixel(int x, int y, uint16_t color565){
	/*
	 * Thinking I'll probably want to cache colors in 255 form, so that we don't lose
	 * so much color depth going from 255->565->255(8 bit)
	 */


	int xEmulated = state.pixels[x][y].x;
    int yEmulated = state.pixels[x][y].y;
    CursorMove(yEmulated,xEmulated);
	if(color565 == 0){
		printf("\033[0m ");
        return;
	}
	int r = ((color565 >> 11) & 0x1F) * 255 / 31;
    int g = ((color565 >> 5) & 0x3F) * 255 / 63;
    int b = (color565 & 0x1F) * 255 / 31;
    printf("\033[48;2;%d;%d;%dm ", r, g, b);
    printf("\033[0m");
}

int PieInitFrameBuffer(){
	EnableRawMode();
    state.killThread = 0;
    return PieInitGraphic();

}

int PieInitGraphic(){

	printf("\033[2J");
	PieRedrawGraphic();
	signal(SIGWINCH,HandleResize);
    signal(SIGINT,  PieCleanExit);
    signal(SIGQUIT, PieCleanExit);
    signal(SIGTERM, PieCleanExit);

    pthread_t refreshThread;
    if(pthread_create(&refreshThread, NULL, PieRefreshThread, NULL) != 0){
        return 1;
    }
    state.refreshThread = refreshThread;
    return 0;
}

int PieCloseGraphic(){

    state.killThread = 1;
    pthread_join(state.refreshThread, NULL);



    signal(SIGWINCH, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT,SIG_DFL);
    signal(SIGTERM,SIG_DFL);
    DisableRawMode();

    printf("\033[2J\033[1;1H");
    //one of these bad boys might screw things up
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

        fflush(stdout);
        usleep(REFRESH60);


    }

    return NULL;

}

void PieUserFBtoState(sense_fb_bitmap_t* userFb){
    state.userFb = userFb;
}


uint16_t RGB255toRGB565(int r,int g,int b){
    //ripped straight from libsense
    r=(float)r / 255.0 * 31.0 + 0.5;
	g=(float)g/ 255.0 * 63.0 + 0.5;
	b=(float)b / 255.0 * 31.0 + 0.5;
	return r<<11|g<<5|b;
}


void CursorMove(int y, int x){
	printf("\033[%d;%dH",y+1,x+1);
}
void PiePrintChar(int y,int x, char c){
	CursorMove(y,x);
	putchar(c);
}
void DisableRawMode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH,&originalTerm);
	printf("\033[?25h");
    printf("\033[0m");
}
void EnableRawMode(){
	tcgetattr(STDIN_FILENO, &originalTerm);
    atexit(DisableRawMode);
    struct termios raw = originalTerm;
    //might experiment without ISIG, since I probably still want to have signals?
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf("\033[?25l");
}


void PieCleanExit(int sig) {
    DisableRawMode();
    PieCloseJoystick();
    PieCloseGraphic();
    exit(0);
}
void HandleResize(int sig) {
    printf("\033[2J");
    PieRedrawGraphic();
}

void PieHandleSegFault(){
    DisableRawMode();
    PieCloseJoystick();
    PieCloseGraphic();
    printf("Segmentation Fault\n");
    exit(1);
}



void PieRedrawGraphic() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        w.ws_col = 80;
        w.ws_row = 24;
    }

    int gridWidth = 33;
    int gridHeight = 17;
    int startX = (w.ws_col - gridWidth) / 2;
    int startY = (w.ws_row - gridHeight) / 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            PiePrintChar(startY + y * 2, startX + x * 4, '+');
            PiePrintChar(startY + y * 2, startX + x * 4 + 1, '-');
            PiePrintChar(startY + y * 2, startX + x * 4 + 2, '-');
            PiePrintChar(startY + y * 2, startX + x * 4 + 3, '-');
        }
        PiePrintChar(startY + y * 2, startX + 32, '+');

        for (int x = 0; x < 8; x++) {
            PiePrintChar(startY + y * 2 + 1, startX + x * 4, '|');

            state.pixels[x][y].x = (startX + x * 4 + 2);
            state.pixels[x][y].y = (startY + (7 - y) * 2 + 1);

            PiePrintChar(startY + y * 2 + 1, startX + x * 4 + 3, ' ');
        }
        PiePrintChar(startY + y * 2 + 1, startX + 32, '|');
    }

    for (int x = 0; x < 8; x++) {
        PiePrintChar(startY + 16, startX + x * 4, '+');
        PiePrintChar(startY + 16, startX + x * 4 + 1, '-');
        PiePrintChar(startY + 16, startX + x * 4 + 2, '-');
        PiePrintChar(startY + 16, startX + x * 4 + 3, '-');
    }
    PiePrintChar(startY + 16, startX + 32, '+');

    fflush(stdout);
}




//Joystick Functions

int PieInitJoystick(){
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

	char buf[3];
	int code;
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	while(!state.killJoystickThread){
		code = 0;
		if (read(STDIN_FILENO, &buf[0], 1) > 0) {
			if (buf[0] == '\x1b') {
				if (read(STDIN_FILENO, &buf[1], 1) > 0 && read(STDIN_FILENO, &buf[2], 1) > 0) {
					if (buf[1] == '[') {
						switch(buf[2]) {
							case 'A': code =  KEY_UP; break;
							case 'B': code =  KEY_DOWN; break;
							case 'C': code =  KEY_RIGHT; break;
							case 'D': code =  KEY_LEFT; break;
						}
					}
				}
			} else if (buf[0] == '\n' || buf[0] == '\r') {
				code =  KEY_ENTER;
			}
		}

		if(code){
			pthread_mutex_lock(&state.joystickPipe.lock);
			state.joystickPipe.keyCode = code;
			state.joystickPipe.read = 0;
			pthread_mutex_unlock(&state.joystickPipe.lock);
		}
		usleep(10000);
	}

	fcntl(STDIN_FILENO, F_SETFL, flags);
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





/*
 *
 *
 * ASM wrapper functions
 *
 *
 */


int openfb(){
	sense_fb_bitmap_t* bm = calloc(1,sizeof(sense_fb_bitmap_t));
    if(bm == NULL){
        return -1;
    }
    PieUserFBtoState(bm);
    if(PieInitFrameBuffer()!=0){
        free(bm);
        return -1;
    }
	return 0;
}

int closefb(){
	free(state.userFb);
	PieCloseGraphic();
	return 0;
}

void setPixel(int x,int y, uint16_t color){
	//int idx1D = (x*8) + y;
	//if(idx1D < 0 || idx1D > 63){PieHandleSegFault();}
	state.userFb->pixel[x][y] = color;
}

int openJoystick(){
    if(PieInitJoystick()!=0)return -1;
    return 1;
}

void closeJoystick(){
    PieCloseJoystick();
}

int getJoystickValue(){

    int code = PieGetJoystickValue();
    switch(code){
        case KEY_UP: return 1;
        case KEY_RIGHT: return 2;
        case KEY_DOWN: return 3;
        case KEY_LEFT: return 4;
        case KEY_ENTER: return 5;
        default: return 0;
    }
    return 0;
}



void sleepy(){

	sleep(3);

}

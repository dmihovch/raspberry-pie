#include "piemulator.h"
#include <string.h>


/*
 * Global State
 */
static PieState state;
static struct termios originalTerm;


/*
 * Libsense Wrapper Functions
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

    PieUserFBtoState(bm);
    if(PieInitFrameBuffer()!=0){
        state.userFb = NULL;
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
	//Stop the refresh thread before freeing the bitmap it reads from.
	PieCloseGraphic();
	state.userFb = NULL;
	free(device->bitmap);
	free(device);
}


//Joystick Wrapper Functions


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
	(void)device;
	//I don't use the timeout here, that may become a problem
	(void)timeout;
	int code = PieGetJoystickValue();
	if(code == -1)return;
	callback(code);
}



/*
 * Private Definitions
 */

#define REFRESH60 16667
#define ROTATION_COUNT 4
#define GRID_WIDTH 33
#define GRID_HEIGHT 17
#define MENU_TEXT "R: rotate Pi"
#define USB_LABEL "USB"
#define ETHERNET_LABEL "ETH"



/*
 * Terminal Intrinsics
 *
 * Every ANSI escape sequence the emulator emits goes through one of these
 * functions, so the rest of the code builds on them instead of inlining
 * escape codes.
 */

static void TerminalClearScreen(void){
	printf("\033[2J");
}

static void TerminalMoveCursor(int row, int col){
	printf("\033[%d;%dH", row + 1, col + 1);
}

static void TerminalResetStyle(void){
	printf("\033[0m");
}

static void TerminalSetBackground(int red, int green, int blue){
	printf("\033[48;2;%d;%d;%dm", red, green, blue);
}

static void TerminalShowCursor(void){
	printf("\033[?25h");
}

static void TerminalHideCursor(void){
	printf("\033[?25l");
}

static void TerminalResetPalette(void){
	printf("\033]104\007");
}

static void TerminalPrintAt(int row, int col, const char* text){
	TerminalMoveCursor(row, col);
	printf("%s", text);
}



/*
 * Orientation
 *
 * The framebuffer is indexed pixel[y][x]. In the default orientation the Pi
 * is held with its USB ports up, so pixel[0][0] is the bottom-left LED, x
 * increases to the right, and y increases upward. Pressing R rotates the Pi
 * 90 degrees clockwise; the USB and Ethernet markers move with it.
 */

static void FramebufferToCell(int x, int y, int rotation, int* cellX, int* cellY){
	switch(rotation){
		case 1: *cellX = y;     *cellY = x;     break;
		case 2: *cellX = 7 - x; *cellY = y;     break;
		case 3: *cellX = 7 - y; *cellY = 7 - x; break;
		default: *cellX = x;    *cellY = 7 - y; break; // rotation 0, USB ports up
	}
}

//Terminal arrow keys mapped to joystick codes for each rotation.
//Index order: up, down, right, left.
static const int arrowKeyCodes[ROTATION_COUNT][4] = {
	{KEY_UP,    KEY_DOWN,  KEY_RIGHT, KEY_LEFT},
	{KEY_LEFT,  KEY_RIGHT, KEY_UP,    KEY_DOWN},
	{KEY_DOWN,  KEY_UP,    KEY_LEFT,  KEY_RIGHT},
	{KEY_RIGHT, KEY_LEFT,  KEY_DOWN,  KEY_UP},
};



/*
 * Emulator Functions
 */




//FrameBuffer Emulator Functions


void PieSetPixel(int x, int y, uint16_t color565){
	/*
	 * Thinking I'll probably want to cache colors in 255 form, so that we don't lose
	 * so much color depth going from 255->565->255(8 bit)
	 */

	int xEmulated = state.pixels[x][y].x;
	int yEmulated = state.pixels[x][y].y;
	TerminalMoveCursor(yEmulated,xEmulated);
	if(color565 == 0){
		TerminalResetStyle();
		printf(" ");
		return;
	}
	int r = ((color565 >> 11) & 0x1F) * 255 / 31;
	int g = ((color565 >> 5) & 0x3F) * 255 / 63;
	int b = (color565 & 0x1F) * 255 / 31;
	TerminalSetBackground(r, g, b);
	printf(" ");
	TerminalResetStyle();
}

int PieInitFrameBuffer(){
	EnableRawMode();
    state.killThread = 0;
    state.rotation = 0;
    return PieInitGraphic();

}

int PieInitGraphic(){

	TerminalClearScreen();
	PieRedrawGraphic();
	signal(SIGWINCH,HandleResize);
    signal(SIGINT,  PieCleanExit);
    signal(SIGQUIT, PieCleanExit);
    signal(SIGTERM, PieCleanExit);
    signal(SIGSEGV, PieHandleSegFault);

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
    signal(SIGSEGV, SIG_DFL);
    DisableRawMode();

    TerminalClearScreen();
    TerminalMoveCursor(0, 0);
    TerminalResetStyle();
    TerminalShowCursor();
    TerminalResetPalette();
    fflush(stdout);



    return 0;
}

void* PieRefreshThread(void* payload){
	(void)payload;
    int x;
    int y;
    while(!state.killThread){
        int rotation = state.rotation;
        for(y = 0; y<8; y++){
            for(x = 0; x<8; x++){
                int cellX;
                int cellY;
                FramebufferToCell(x, y, rotation, &cellX, &cellY);
                PieSetPixel(cellX, cellY, state.userFb->pixel[y][x]);
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
	TerminalMoveCursor(y, x);
}
void PiePrintChar(int y,int x, char c){
	TerminalMoveCursor(y, x);
	putchar(c);
}
void DisableRawMode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH,&originalTerm);
	TerminalShowCursor();
	TerminalResetStyle();
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
    TerminalHideCursor();
}


void PieCleanExit(int sig) {
    (void)sig;
    DisableRawMode();
    PieCloseJoystick();
    PieCloseGraphic();
    exit(0);
}
void HandleResize(int sig) {
    (void)sig;
    TerminalClearScreen();
    PieRedrawGraphic();
}

void PieHandleSegFault(int sig){
    (void)sig;
    DisableRawMode();
    PieCloseJoystick();
    PieCloseGraphic();
    printf("Segmentation Fault\n");
    exit(1);
}



static void DrawRotationMenu(int startX, int startY){
	if(startX < (int)strlen(MENU_TEXT) || startY < 1) return;
	TerminalPrintAt(0, 0, MENU_TEXT);
}

static void DrawEdgeLabel(int row, int col, const char* label, int terminalWidth, int terminalHeight){
	if(row < 0 || row >= terminalHeight) return;
	if(col < 0 || col + (int)strlen(label) > terminalWidth) return;
	TerminalPrintAt(row, col, label);
}

static void DrawEdgeMarkers(int startX, int startY, int rotation, int terminalWidth, int terminalHeight){
	//The USB ports sit on the top edge in the default orientation and the
	//Ethernet port on the right edge; both rotate clockwise with the Pi.
	int topRow = startY - 1;
	int bottomRow = startY + GRID_HEIGHT;
	int leftCol = startX - (int)strlen(USB_LABEL) - 1;
	int rightCol = startX + GRID_WIDTH + 1;
	int centerCol = startX + (GRID_WIDTH - (int)strlen(USB_LABEL)) / 2;
	int centerRow = startY + GRID_HEIGHT / 2;

	switch(rotation){
		case 0:
			DrawEdgeLabel(topRow, centerCol, USB_LABEL, terminalWidth, terminalHeight);
			DrawEdgeLabel(centerRow, rightCol, ETHERNET_LABEL, terminalWidth, terminalHeight);
			break;
		case 1:
			DrawEdgeLabel(centerRow, rightCol, USB_LABEL, terminalWidth, terminalHeight);
			DrawEdgeLabel(bottomRow, centerCol, ETHERNET_LABEL, terminalWidth, terminalHeight);
			break;
		case 2:
			DrawEdgeLabel(bottomRow, centerCol, USB_LABEL, terminalWidth, terminalHeight);
			DrawEdgeLabel(centerRow, leftCol, ETHERNET_LABEL, terminalWidth, terminalHeight);
			break;
		case 3:
			DrawEdgeLabel(centerRow, leftCol, USB_LABEL, terminalWidth, terminalHeight);
			DrawEdgeLabel(topRow, centerCol, ETHERNET_LABEL, terminalWidth, terminalHeight);
			break;
	}
}

void PieRedrawGraphic() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        w.ws_col = 80;
        w.ws_row = 24;
    }

    int startX = (w.ws_col - GRID_WIDTH) / 2;
    int startY = (w.ws_row - GRID_HEIGHT) / 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    DrawRotationMenu(startX, startY);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            PiePrintChar(startY + y * 2, startX + x * 4, '+');
            PiePrintChar(startY + y * 2, startX + x * 4 + 1, '-');
            PiePrintChar(startY + y * 2, startX + x * 4 + 2, '-');
            PiePrintChar(startY + y * 2, startX + x * 4 + 3, '-');
        }
        PiePrintChar(startY + y * 2, startX + GRID_WIDTH - 1, '+');

        for (int x = 0; x < 8; x++) {
            PiePrintChar(startY + y * 2 + 1, startX + x * 4, '|');

            state.pixels[x][y].x = (startX + x * 4 + 2);
            state.pixels[x][y].y = (startY + (7 - y) * 2 + 1);

            PiePrintChar(startY + y * 2 + 1, startX + x * 4 + 3, ' ');
        }
        PiePrintChar(startY + y * 2 + 1, startX + GRID_WIDTH - 1, '|');
    }

    for (int x = 0; x < 8; x++) {
        PiePrintChar(startY + GRID_HEIGHT - 1, startX + x * 4, '+');
        PiePrintChar(startY + GRID_HEIGHT - 1, startX + x * 4 + 1, '-');
        PiePrintChar(startY + GRID_HEIGHT - 1, startX + x * 4 + 2, '-');
        PiePrintChar(startY + GRID_HEIGHT - 1, startX + x * 4 + 3, '-');
    }
    PiePrintChar(startY + GRID_HEIGHT - 1, startX + GRID_WIDTH - 1, '+');

    DrawEdgeMarkers(startX, startY, state.rotation, w.ws_col, w.ws_row);

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

	(void)payload;

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
						int arrowIndex;
						switch(buf[2]) {
							case 'A': arrowIndex = 0; break;
							case 'B': arrowIndex = 1; break;
							case 'C': arrowIndex = 2; break;
							case 'D': arrowIndex = 3; break;
							default: arrowIndex = -1; break;
						}
						if (arrowIndex >= 0) {
							code = arrowKeyCodes[state.rotation][arrowIndex];
						}
					}
				}
			} else if (buf[0] == 'r' || buf[0] == 'R') {
				state.rotation = (state.rotation + 1) % ROTATION_COUNT;
				if (state.userFb != NULL) {
					PieRedrawGraphic();
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
 * Debug and Util Functions
 */



void PieDebug(){
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            fprintf(stderr,"x:%d y:%d\n",state.pixels[i][j].x,state.pixels[i][j].y);
        }
    }
}




/*
 * Assembly API
 */

int openfb(){
	sense_fb_bitmap_t* bm = calloc(1,sizeof(sense_fb_bitmap_t));
    if(bm == NULL){
        return -1;
    }
    PieUserFBtoState(bm);
    if(PieInitFrameBuffer()!=0){
        state.userFb = NULL;
        free(bm);
        return -1;
    }
	return 0;
}

int closefb(){
	//Stop the refresh thread before freeing the bitmap it reads from.
	PieCloseGraphic();
	free(state.userFb);
	state.userFb = NULL;
	return 0;
}

void setPixel(int x,int y, uint16_t color){
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
}
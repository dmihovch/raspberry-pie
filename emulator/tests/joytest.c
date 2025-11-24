#include "../lib/piemulator.h"
#include <stdio.h>
int x = 0;
int y = 0;
int running = 1;
void drawdot(pi_framebuffer_t* fb, int x, int y){
	fb->bitmap->pixel[x][y] = 0xFFFF;

}


int clamp(int coord){
	if(coord < 0) return 0;
	if(coord > 7) return 7;
	return coord;
}

void handler(unsigned int code){
	fprintf(stderr,"code = %d\n",code);
	switch(code){
		case KEY_UP: {y = clamp(y+1); break;};
	 	case KEY_DOWN:{ y = clamp(y-1); break;};
		case KEY_LEFT:{ x = clamp(x-1);break;};
		case KEY_RIGHT:{ x = clamp(x+1);break;};
		case KEY_ENTER: {running = 0; break;}
		fprintf(stderr,"running in handler %d\n", running);
	}
}

int main(){

	pi_framebuffer_t* fb = getFrameBuffer();
	pi_joystick_t* js = getJoystickDevice();

	if(fb == NULL || js == NULL){
		return 1;
	}
	fprintf(stderr,"\n\n\nNEW START OF PROGRAM\n\n\n");

	while(running){
		pollJoystick(js, handler, 0);
		fb->bitmap->pixel[x][y] = 0xFFFF;
		fb->bitmap->pixel[x][y] = 0;
	}


	freeFrameBuffer(fb);
	freeJoystick(js);

	return 0;



}

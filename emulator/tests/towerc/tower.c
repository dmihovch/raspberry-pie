
#include "../../lib/piemulator.h"
int running = 1;
int x = 0;
int y = 0;




int clamp(int coord){
	if(coord < 0) return 0;
	if(coord > 7) return 7;
	return coord;
}



void handler(unsigned int code){
	switch(code){
		case KEY_UP: {y = clamp(y+1); break;};
	 	case KEY_DOWN:{ y = clamp(y-1); break;};
		case KEY_LEFT:{ x = clamp(x-1);break;};
		case KEY_RIGHT:{ x = clamp(x+1);break;};
		case KEY_ENTER: {running = 0; break;}
	}
}



int main(void){

	pi_framebuffer_t* fb = getFrameBuffer();
	pi_joystick_t* js = getJoystickDevice();


	while(running){
		pollJoystick(js, handler, 0);
		int out = 1;
		clearFrameBuffer(fb, 0);
		fb->bitmap->pixel[x][y] = 0xFFFF;
		for(int i = y-1; i >= 0; i--){
			for(int j = x-out; j<=x+out; j++){
				int jc = clamp(j);
				fb->bitmap->pixel[i][jc] = 0xFFFF;
			}
		}
	}

	freeFrameBuffer(fb);
	freeJoystick(js);


}

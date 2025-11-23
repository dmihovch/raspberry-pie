#include "include/piscreen.h"

//for testing
int main(){
	pi_framebuffer_t* fb = getFrameBuffer();
	if(fb == NULL){
		return 1;
	}

	uint16_t red = getColor(255, 0, 0);
	uint16_t purp = getColor(255,0,255);
	uint16_t yellow = getColor(255, 255, 0);
	uint16_t green = getColor(0,255,0);

	uint16_t colors[4] = {red,purp,yellow,green};

	fb->bitmap->pixel[0][0] = red;
	fb->bitmap->pixel[0][7] = purp;
	fb->bitmap->pixel[7][0] = yellow;
	fb->bitmap->pixel[7][7] = green;

	sleep(4);
	clearFrameBuffer(fb, 0);
	freeFrameBuffer(fb);
	return 0;
}

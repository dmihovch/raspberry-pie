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

	for(int i = 0; i<4; i++){

		fb->bitmap->pixel[i][0] = colors[i];

	}
	sleep(4);
	clearFrameBuffer(fb, 0);
	freeFrameBuffer(fb);
	return 0;
}

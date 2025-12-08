#include "../../lib/piemulator.h"


int main(){

	pi_framebuffer_t* fb = getFrameBuffer();

	fb->bitmap->pixel[1][0] = 0xFFFF;

	sleep(3);
	freeFrameBuffer(fb);

	return 0;

}

#include <piemulator.h>

int main(){

	pi_framebuffer_t *fb=getFrameBuffer();
	sense_fb_bitmap_t *bm=fb->bitmap;

  bm->pixel[1][0] = 0xFFFF;
  bm->pixel[0][0] = 0xF00F;
  bm->pixel[7][7] = 0x0FFF;
  sleep(2);

	freeFrameBuffer(fb);
  
}

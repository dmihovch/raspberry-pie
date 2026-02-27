#include "scroll.h"
pi_framebuffer_t *fb = NULL;
sense_fb_bitmap_t *bm = NULL;

float m[22][2] = {
	{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},
	{1,6},{2,5},{3,4},{4,4},{5,5},{6,6},{7,7},
	{7,6},{7,5},{7,4},{7,3},{7,2},{7,1},{7,0}
};//x,y
//I know this is all terrible. I hate it too
float d[24][2] = {
	{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{1,7},{2,7},{3,7},{4,7},{5,7},
	{1,0},{2,0},{3,0},{4,0},{5,0},{6,1},{7,2},{7,3},{7,4},{7,5},{6,6}
};//x,y

void open_display(void){
	fb=getFrameBuffer();
	bm=fb->bitmap;
}
void close_display(void){
	int nPhase = sleep(1);
	if(nPhase == 0){
		clearFrameBuffer(fb,BLACK);
		sleep(1);
	}
	freeFrameBuffer(fb);
}
void display_letter(char letter, int xOffset, int yOffset){
	clearFrameBuffer(fb,BLACK);
	if(letter == 'd'){
		for(int i = 0; i<24; i++){
			d[i][0] = checkOffset((d[i][0] + (float)xOffset/10));
			d[i][1] = checkOffset((d[i][1] + (float)yOffset/10));
			bm->pixel[(int)d[i][0]][(int)d[i][1]]=WHITE;
		}
	}
	if(letter == 'm'){
		for(int p = 0; p<22; p++){
			m[p][0] = checkOffset((m[p][0] + (float)xOffset/10));
			m[p][1] = checkOffset((m[p][1] + (float)yOffset/10));
			bm->pixel[(int)m[p][0]][(int)m[p][1]]=WHITE;
		}
	}
}
void clear_display(void){
	//sleep(1);
	clearFrameBuffer(fb,BLACK);
}
float checkOffset(float off){
	float ret = fmod(off,8.0);
	if(ret < 0){
		ret += 8;
	}
	return ret;
}

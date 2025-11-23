#include <stdio.h>
#include <unistd.h>
#include "../include/piscreen.h"

#define BLACK 0x0000
#define WHITE 0xFFFF
int main(void){
	pi_framebuffer_t *fb=getFrameBuffer();
	sense_fb_bitmap_t *bm=fb->bitmap;

	//Start initial D
	int i;
	for(i=0;i<8;i++){
		bm->pixel[7][i]=WHITE;
	}
	bm->pixel[6][0]=WHITE;
	bm->pixel[5][0]=WHITE;
	bm->pixel[4][0]=WHITE;
	bm->pixel[3][0]=WHITE;
	bm->pixel[2][0]=WHITE;
	
	bm->pixel[6][7]=WHITE;
	bm->pixel[5][7]=WHITE;
	bm->pixel[4][7]=WHITE;
	bm->pixel[3][7]=WHITE;
	bm->pixel[2][7]=WHITE;

	bm->pixel[1][1]=WHITE;
	bm->pixel[0][2]=WHITE;
	bm->pixel[0][5]=WHITE;
	bm->pixel[1][6]=WHITE;
	bm->pixel[0][3]=WHITE;
	bm->pixel[0][4]=WHITE;
	int nPhase = sleep(1);
	if (nPhase == 0)
	{
		clearFrameBuffer(fb,BLACK);
        	sleep(1);
        }
	//End Initial D
	//Start Initial M
	int j;
	for(j=0;j<8;j++){
	bm->pixel[7][j]=WHITE;
	bm->pixel[0][j]=WHITE;
	}
	bm->pixel[6][1]=WHITE;
	bm->pixel[5][2]=WHITE;
	bm->pixel[4][3]=WHITE;
	bm->pixel[3][3]=WHITE;
	bm->pixel[2][2]=WHITE;
	bm->pixel[1][1]=WHITE;
	int nPhase2 = sleep(1);
	if (nPhase2 == 0)
        {
                clearFrameBuffer(fb,BLACK);
                sleep(1);
        }
	freeFrameBuffer(fb);
	return 0;
}

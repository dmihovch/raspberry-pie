#include "display.h"
#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREEN 0x0F0F
#define RED 0xF00F
#define BLUE 0x00FF

pi_framebuffer_t *fb = NULL;
sense_fb_bitmap_t *bm = NULL;
int open_display(void){
	if(fb!=NULL){
		return 0;
	}//this might cause problems if I am wrong lol!
	fb=getFrameBuffer();
	bm=fb->bitmap;
	if(fb==NULL){
		return 0;
	}
	return 1;
	//this might work
}

void display_time(int hours,int minutes,int seconds){
	display_colons();
	display_hours(hours);
	display_minutes(minutes);
	display_seconds(seconds);
}

void display_colons(void){
	bm->pixel[1][1]=WHITE;
	bm->pixel[2][1]=WHITE;
	bm->pixel[1][2]=WHITE;
	bm->pixel[2][2]=WHITE;

	bm->pixel[1][4]=WHITE;
	bm->pixel[1][5]=WHITE;
	bm->pixel[2][4]=WHITE;
	bm->pixel[2][5]=WHITE;

	bm->pixel[4][1]=WHITE;
	bm->pixel[4][2]=WHITE;
	bm->pixel[5][1]=WHITE;
	bm->pixel[5][2]=WHITE;

	bm->pixel[4][4]=WHITE;
	bm->pixel[4][5]=WHITE;
	bm->pixel[5][4]=WHITE;
	bm->pixel[5][5]=WHITE;
}

void display_hours(int hours){
	int binaryRev[6];
	int binary[6];
	int i = 0;
	int num = hours;
	while(num > 0){
		binaryRev[i] = num % 2;
		num/=2;
		i++;
	}
	int i1 = 0;
	for(int j = i - 1; j>=0; j--){
		binary[i1] = binaryRev[j];
		i1++;
	}
	for(int p=0;p<6;p++){
		if(binary[p] == 1){
			bm->pixel[p][6]=BLUE;
		}
		else if(binary[p] == 0){
			bm->pixel[p][6]=BLACK;
		}
	}

}

void display_minutes(int minutes){
	int binaryRev[6];
	int binary[6];
	int i = 0;
	int num = minutes;
	while(num > 0){
		binaryRev[i] = num % 2;
		num/=2;
		i++;
	}
	int i1 = 0;
	for(int j = i - 1; j>=0; j--){
		binary[i1] = binaryRev[j];
		i1++;
	}
	for(int p=0;p<6;p++){
		if(binary[p] == 1){
			bm->pixel[p][3]=GREEN;
		}
		else if(binary[p] == 0){
			bm->pixel[p][3]=BLACK;
		}
	}

}

void display_seconds(int seconds){
	int binaryRev[6];
	int binary[6];
	int i = 0;
	int num = seconds;
	while(num > 0){
		binaryRev[i] = num % 2;
		num/=2;
		i++;
	}
	int i1 = 0;
	for(int j = i - 1; j>=0; j--){
		binary[i1] = binaryRev[j];
		i1++;
	}
	for(int p=0;p<6;p++){
		if(binary[p] == 1){
			bm->pixel[p][0]=RED;
		}
		else if(binary[p] == 0){
			bm->pixel[p][0]=BLACK;
		}
	}

}
void close_display(void){
	//this is maybe working now?
	int nPhase = sleep(1);
	if(nPhase == 0){
		clearFrameBuffer(fb,BLACK);
		sleep(1);
	}
	freeFrameBuffer(fb);
}

//i don't actually know if any of this actually works or not loL!

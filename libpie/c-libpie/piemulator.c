#include "piemulator.h"
#include <stdio.h>




/*
 *
 *
 * Libsense Wrapper Functions
 *
 *
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
	free(device->bitmap);
	free(device);
	PieCloseGraphic();
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
	int code = PieGetJoystickValue();
	//I don't use the timeout here, that may become a problem
	if(code == -1)return;
	callback(code);
}



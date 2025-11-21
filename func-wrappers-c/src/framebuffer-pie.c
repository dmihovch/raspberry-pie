#include "../include/framebuffer-pie.h"
#include "../../emulator/include/piscreen.h" //idk about this yet

uint16_t getColor(int red, int green, int blue){
    ColorRaw color = CreateColorRaw(red, green, blue);
    HandleGetColor(color);
    return color.rgb565;
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

    SendUserFBtoGlobalState(ufb);
    if(InitPieFrameBuffer()!=0){
        free(ufb->bitmap);
        free(ufb);
        return NULL;
    }

    return ufb;

}

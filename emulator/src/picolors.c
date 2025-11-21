#include "../include/picolors.h"




uint16_t RGB255toRGB565(int r,int g,int b){
    //ripped straight from libsense
    r=(float)r / 255.0 * 31.0 + 0.5;
	g=(float)g/ 255.0 * 63.0 + 0.5;
	b=(float)b / 255.0 * 31.0 + 0.5;
	return r<<11|g<<5|b;
}
ColorRaw CreateColorRaw(int r,int g,int b){
    ColorRaw color;
    color.r = (r*1000)/255;
    color.g = (g*1000)/255;
    color.b = (b*1000)/255;
    color.type = CURSES;
    color.rgb565 = RGB255toRGB565(r,g,b);
    return color;
}

ColorCacheEntry* CreateColorCacheEntry(ColorRaw color, int nextCursesIdx){

    ColorCacheEntry* cce = malloc(sizeof(ColorCacheEntry));
    if(cce == NULL) return NULL;

    cce->color = color;
    cce->cursesColorIdx = nextCursesIdx;
    cce->next = NULL;
    return cce;

}

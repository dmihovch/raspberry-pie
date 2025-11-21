#include "../include/picolors.h"





	int rScaled, gScaled, bScaled, xGrid, yGrid;
    rScaled = (r * 1000) / 255;
    gScaled = (g * 1000) / 255;
    bScaled = (b * 1000) / 255;

    xGrid = fb->pixels[x][y].x;
    yGrid = fb->pixels[x][y].y;

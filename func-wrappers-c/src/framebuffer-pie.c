#include "../include/framebuffer-pie.h"
#include "../../emulator/include/picolors.h" //idk about this yet
#include <stdint.h>

uint16_t getColor(int red, int green, int blue){

    ColorRaw color = CreateColorRaw(red, green, blue);





    return color.rgb565;

}

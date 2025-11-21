#include "../include/framebuffer-pie.h"
#include "../../emulator/include/piscreen.h" //idk about this yet
#include <stdint.h>

uint16_t getColor(int red, int green, int blue){
    ColorRaw color = CreateColorRaw(red, green, blue);
    HandleGetColor(color);
    return color.rgb565;
}

#include "../include/framebuffer-pie.h"
#include <stdint.h>

uint16_t getColor(int red, int green, int blue){




	int sr,sg,sb;
	sr=(float)red / 255.0 * 31.0 + 0.5;
	sg=(float)green/ 255.0 * 63.0 + 0.5;
	sb=(float)blue / 255.0 * 31.0 + 0.5;
	return sr<<11|sg<<5|sb;


}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "piemulator.h"

const char *text[5] = {
    "###  #  ###       ##  ##  ## ",
    "  #  #  # #  #    # # # # # #",
    "###  #  # #       ##  ##  ## ",
    "#    #  # #  #    # # # # # #",
    "###  #  ###       ##  # # ## "
};

int main(void) {
    pi_framebuffer_t *fb = getFrameBuffer();
    if (fb == NULL) {
        return 1;
    }

    uint16_t color = getColor(255, 255, 255);
    int text_width = strlen(text[0]);
    int x_offset = 0;

    while (1) {
        clearFrameBuffer(fb, 0x0000);

        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < text_width; c++) {
                int sx = x_offset - c;
                int sy = r + 1;

                if (sx >= 0 && sx < 8 && sy >= 0 && sy < 8) {
                    if (text[r][c] == '#') {
                        fb->bitmap->pixel[sx][sy] = color;
                    }
                }
            }
        }

        usleep(150000);
        x_offset++;

        if (x_offset > text_width + 8) {
            x_offset = 0;
        }
    }

    freeFrameBuffer(fb);
    return 0;
}

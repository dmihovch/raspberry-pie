#include "../../lib/piemulator.h"
#include <math.h>
#include <unistd.h>

// Convert HSV to RGB (for smooth rainbow colors)
// h: 0-360, s: 0-1, v: 0-1
void hsvToRgb(float h, float s, float v, int* r, int* g, int* b) {
    int i;
    float f, p, q, t;

    if (s == 0) {
        *r = *g = *b = (int)(v * 255);
        return;
    }

    h /= 60;
    i = (int)h;
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
        case 0: *r = (int)(v * 255); *g = (int)(t * 255); *b = (int)(p * 255); break;
        case 1: *r = (int)(q * 255); *g = (int)(v * 255); *b = (int)(p * 255); break;
        case 2: *r = (int)(p * 255); *g = (int)(v * 255); *b = (int)(t * 255); break;
        case 3: *r = (int)(p * 255); *g = (int)(q * 255); *b = (int)(v * 255); break;
        case 4: *r = (int)(t * 255); *g = (int)(p * 255); *b = (int)(v * 255); break;
        default: *r = (int)(v * 255); *g = (int)(p * 255); *b = (int)(q * 255); break;
    }
}

int main() {
    pi_framebuffer_t* fb = getFrameBuffer();
    if (fb == NULL) {
        return 1;
    }

    // Run for about 10 seconds with high frame rate
    int frames = 600;  // 60 fps * 10 seconds
    float time_step = 0.016f;  // ~60fps

    for (int frame = 0; frame < frames; frame++) {
        float time = frame * time_step;

        // Create a rainbow wave that sweeps across the display
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                // Calculate distance from center for radial wave
                float center_x = 3.5f;
                float center_y = 3.5f;
                float dx = x - center_x;
                float dy = y - center_y;
                float distance = sqrtf(dx * dx + dy * dy);

                // Create wave pattern with multiple frequencies for complexity
                float wave1 = sinf(distance * 0.8f - time * 3.0f) * 0.5f + 0.5f;
                float wave2 = sinf((x + y) * 0.5f - time * 2.0f) * 0.3f + 0.3f;
                float wave3 = sinf(time * 1.5f) * 0.2f + 0.2f;

                // Combine waves for rich color variation
                float hue = (wave1 * 360.0f + wave2 * 180.0f + wave3 * 90.0f);
                hue = fmodf(hue, 360.0f);
                if (hue < 0) hue += 360.0f;

                // Vary saturation and value for more color diversity
                float saturation = 0.7f + wave2 * 0.3f;
                float value = 0.5f + wave1 * 0.5f;

                // Convert to RGB
                int r, g, b;
                hsvToRgb(hue, saturation, value, &r, &g, &b);

                // Clamp values
                if (r < 0) r = 0; if (r > 255) r = 255;
                if (g < 0) g = 0; if (g > 255) g = 255;
                if (b < 0) b = 0; if (b > 255) b = 255;

                // Set pixel with new color (tests color cache)
                fb->bitmap->pixel[y][x] = getColor(r, g, b);
            }
        }

        // Small delay to control frame rate (~60fps)
        usleep(16667);  // ~16.67ms per frame
    }

    // Clear and cleanup
    clearFrameBuffer(fb, 0);
    freeFrameBuffer(fb);
    return 0;
}

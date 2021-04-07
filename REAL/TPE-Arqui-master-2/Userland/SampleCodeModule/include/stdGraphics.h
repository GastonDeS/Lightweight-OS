#ifndef STD_GRAPHICS_H
#define STD_GRAPHICS_H
#include <stdint.h>
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BASE_CHAR_WIDTH 8
#define BASE_CHAR_HEIGHT 16

void drawString(int x, int y, char * buffer, int dim, uint64_t fontColor, uint64_t backgroundColor, int fontSize, int alphaBackground);
void clearScreen(uint64_t color);
void drawRect(int xi, int yi, int width, int height, uint64_t color);
#endif

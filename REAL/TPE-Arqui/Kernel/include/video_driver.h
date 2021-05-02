#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H
#include <stdint.h>

typedef uint64_t color;

void clearDisplay(color);
void drawLine(uint64_t xStart, uint64_t yStart, uint64_t xEnd, uint64_t yEnd, color col);
void drawMatrix(uint64_t x, uint64_t y, color *mat, uint64_t height, uint64_t width,uint64_t size);
void drawChar(uint64_t x, uint64_t y, uint8_t character, uint64_t fontSize, color fontColor, color backgroundColor, uint8_t alphaBackground);
void drawPixel(uint64_t x, uint64_t y, color col);
void drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, color col);
void writeStr(char *buffer,uint64_t size,uint64_t x,uint64_t y, uint64_t fontColor, uint64_t backgroundColor, uint64_t fontSize, uint64_t alphaBackground);
uint64_t getCharWidth();
uint64_t getCharHeight();
uint64_t getScreenHeightRes();
uint64_t getScreenWidthRes();
#endif

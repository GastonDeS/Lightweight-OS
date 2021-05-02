#ifndef VIDEO_DRIVER
#define VIDEO_DRIVER
#include <video_driver.h>
#include <stdint.h>
#include <font.h>

static uint8_t legalCoordinates(uint64_t x, uint64_t y);
static void drawVerticalLine(uint64_t x, uint64_t yStart, uint64_t yEnd, color col);

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));


struct vbe_mode_info_structure * screenData = (struct vbe_mode_info_structure *)0x0000000000005C00;

void clearDisplay(color col) {
  uint64_t totalPixels = screenData->width*screenData->height;
  uint8_t * curpos = screenData->framebuffer;
  uint8_t b = col & 0x0000FF;
	uint8_t g = (col >> 8) & 0x0000FF;
	uint8_t r = (col >> 16) & 0x0000FF;
  for (uint64_t i = 0; i < totalPixels; i++) {
    *curpos = b;
    curpos++;
    *curpos = g;
    curpos++;
    *curpos = r;
    curpos++;
  }
}

void drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, color col) {
  if (legalCoordinates(x, y)) {
    uint8_t b = col & 0x0000FF;
  	uint8_t g = (col >> 8) & 0x0000FF;
  	uint8_t r = (col >> 16) & 0x0000FF;
    for (uint64_t i = 0; i < height; i++) {
      for (uint64_t j = 0; j < width; j++) {
        if (legalCoordinates(x+j, y+i)) {
          uint8_t * pos = (uint8_t*)(screenData->framebuffer+ ((y+i) * screenData->width + (x+j))*3);
          *pos = b;
          pos++;
          *pos = g;
          pos++;
          *pos = r;
        }
      }
    }
  }
}

void drawLine(uint64_t xStart, uint64_t yStart, uint64_t xEnd, uint64_t yEnd, color col) {
  if (!legalCoordinates(xStart, yStart) || !legalCoordinates(xEnd, yEnd)) return;
  if(xStart==xEnd){
    drawVerticalLine(xStart, yStart, yEnd, col);
  } else {
    uint8_t b = col & 0x0000FF;
  	uint8_t g = (col >> 8) & 0x0000FF;
  	uint8_t r = ( col >> 16) & 0x0000FF;
    float m = (yEnd-yStart)/(xEnd-xStart);
    float b0 = 1.0f * yStart - m*xStart;
    for(uint64_t i = xStart; i <= xEnd; i++){
          uint64_t y = (uint64_t) (m * i + b0);
          uint8_t * pos = (uint8_t *)(screenData->framebuffer+ (y * screenData->width + i) * 3);
          *pos = b;
          pos++;
          *pos = g;
          pos++;
          *pos = r;
    }
  }
}

static void drawVerticalLine(uint64_t x, uint64_t yStart, uint64_t yEnd, color col) {
  if (!legalCoordinates(x, yStart) || !legalCoordinates(x, yEnd)) return;
  uint8_t b = col & 0x0000FF;
  uint8_t g = (col >> 8) & 0x0000FF;
  uint8_t r = ( col >> 16) & 0x0000FF;
  for (uint64_t i = yStart; i <= yEnd; i++) {
    uint8_t * pos = (uint8_t *)(screenData->framebuffer+ (i * screenData->width + x) * 3);
    *pos = b;
    pos++;
    *pos = g;
    pos++;
    *pos = r;
  }
}

static uint8_t legalCoordinates(uint64_t x, uint64_t y) {
  if (x>=screenData->width || y >= screenData->height || x < 0 || y < 0){
		return 0;
	}
  return 1;
}

void writeStr(char *buffer,uint64_t size,uint64_t x,uint64_t y, uint64_t fontColor, uint64_t backgroundColor, uint64_t fontSize, uint64_t alphaBackground) {
  uint64_t xOffset = 0;
  for (uint64_t i = 0; i < size && buffer[i] != 0; i++) {
    char ch = buffer[i];
    drawChar(x + xOffset, y, ch, fontSize, fontColor, backgroundColor, alphaBackground);
    xOffset += getCharWidth() * fontSize;
  }
}

void drawChar(uint64_t x, uint64_t y, uint8_t character, uint64_t fontSize, color fontColor, color backgroundColor, uint8_t alphaBackground){
	uint64_t aux_x = x;
	uint64_t aux_y = y;
  uint8_t bitIsPresent;

	unsigned char * toDraw = charBitmap(character);

	for (uint8_t i = 0; i < CHAR_HEIGHT; i++){
		for (uint8_t j = 0; j < CHAR_WIDTH; j++){
			bitIsPresent = (1<< (CHAR_WIDTH-j)) & toDraw[i];

			if (bitIsPresent){
				drawRectangle(aux_x, aux_y, fontSize, fontSize, fontColor);
			} else if (alphaBackground == 0) {
				drawRectangle(aux_x, aux_y, fontSize, fontSize, backgroundColor);
			}
			aux_x+=fontSize;
		}
		aux_x = x;
		aux_y += fontSize;
	}
}

void drawMatrix(uint64_t x, uint64_t y, color *mat, uint64_t width, uint64_t height,uint64_t size){

  for (int i = 0; i < height*size; i++){
	    for (int j = 0; j < width*size; j++){
        if (!legalCoordinates(x+j, y+i) ) continue;
        if (mat[(i/size)*width+(j/size)]==0x01000000) continue;
        color col = mat[(i/size)*width+(j/size)];
        uint8_t b = col & 0x0000FF;
        uint8_t g = (col >> 8 )& 0x0000FF;
        uint8_t r = ( col >> 16) & 0x0000FF;
        uint8_t * pos = (uint8_t *)(screenData->framebuffer+ ((y+i) * screenData->width + (x+j)) * 3);
        *pos = b;
        pos++;
        *pos = g;
        pos++;
        *pos = r;
	    }
	}
}

void drawPixel(uint64_t x, uint64_t y, color col) {
		if (!legalCoordinates(x, y)) return;
    uint8_t * curpos = screenData->framebuffer;
    curpos+=(x+screenData->width*y)*3;

    uint8_t b = col & 0x0000FF;
    uint8_t g = (col >> 8 )& 0x0000FF;
    uint8_t r = ( col >> 16) & 0x0000FF;

    *curpos = b;
    curpos++;
    *curpos = g;
    curpos++;
    *curpos = r;
    curpos++;
}

uint64_t getCharWidth() {
  return CHAR_WIDTH;
}

uint64_t getCharHeight() {
  return CHAR_HEIGHT;
}

uint64_t getScreenWidthRes() {
	return screenData->width;
}

uint64_t getScreenHeightRes() {
	return screenData->height;
}


#endif

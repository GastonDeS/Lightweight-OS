// #ifndef VIDEO_DRIVER
// #define VIDEO_DRIVER
// #include <stdint.h>
// #include <video_driver.h>
// #include <font.h>
// static int CHAR_SPACE = 2;
//
// struct vbe_mode_info_structure {
// 	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
// 	uint8_t window_a;			// deprecated
// 	uint8_t window_b;			// deprecated
// 	uint16_t granularity;		// deprecated; used while calculating bank numbers
// 	uint16_t window_size;
// 	uint16_t segment_a;
// 	uint16_t segment_b;
// 	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
// 	uint16_t pitch;			// number of bytes per horizontal line
// 	uint16_t width;			// width in pixels
// 	uint16_t height;			// height in pixels
// 	uint8_t w_char;			// unused...
// 	uint8_t y_char;			// ...
// 	uint8_t planes;
// 	uint8_t bpp;			// bits per pixel in this mode
// 	uint8_t banks;			// deprecated; total number of banks in this mode
// 	uint8_t memory_model;
// 	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
// 	uint8_t image_pages;
// 	uint8_t reserved0;
//
// 	uint8_t red_mask;
// 	uint8_t red_position;
// 	uint8_t green_mask;
// 	uint8_t green_position;
// 	uint8_t blue_mask;
// 	uint8_t blue_position;
// 	uint8_t reserved_mask;
// 	uint8_t reserved_position;
// 	uint8_t direct_color_attributes;
//
// 	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
// 	uint32_t off_screen_mem_off;
// 	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
// 	uint8_t reserved1[206];
// } __attribute__ ((packed));
//
//
// struct vbe_mode_info_structure * screenData = (struct vbe_mode_info_structure *)0x0000000000005C00;
//
//
// void clearDisplay(uint64_t color){
// 	uint8_t b = color & 0x0000FF;
// 	uint8_t g = (color >> 8 )& 0x0000FF;
// 	uint8_t r = ( color >> 16) & 0x0000FF;
// 	// for (int i = 0; i < screenData->width; i++){
// 	// 	for (int j = 0; j < screenData->height; j++){
// 	// 		uint8_t * curpos = screenData->framebuffer;
// 	//     curpos+=(i+screenData->width*j)*3;
// 	//
// 	//     *curpos = b;
// 	//     curpos++;
// 	//     *curpos = g;
// 	//     curpos++;
// 	//     *curpos = r;
// 	//     curpos++;
// 	// 	}
// 	// }
// 	uint64_t totalPixels = screenData->width*screenData->height;
// 	uint8_t * curpos = screenData->framebuffer;
// 	for (int i = 0; i < totalPixels; i++) {
//
// 		*curpos = b;
// 		*(curpos+1) = g;
// 		*(curpos+2) = r;
// 		curpos+=3;
// 	}
// }
//
// void drawSquare(uint64_t x, uint64_t y, uint64_t size, uint64_t color){
// 	if (x>screenData->width || y > screenData->height || x < 0 || y < 0){
// 		return ;
// 	}
// 	for (int i = 0; i < size; i++){
// 		for (int j = 0; j < size; j++){
// 			drawPixel(x+i,j+y,color);
// 		}
// 	}
// }
//
// void drawLine(int xi,int yi,int xf,int yf,uint64_t color){
// 	if(xi==xf){
//         for(int i = yi ; i < yf ; i++){
//             drawPixel(xi,i,color);
//         }
//         return;
//     }
//     int m = (yf-yi)/(xf-xi);
//     int b = yi - m*xi;
//     for(int i = xi ; i < xf ; i++){
//           drawPixel(i,m*i+b,color);
//     }
// }
//
// void drawMatrix(uint64_t x, uint64_t y, uint64_t *mat, uint64_t height, uint64_t width,uint64_t size){
// 	for (int i = 0; i < height*size; i++){
// 	    for (int j = 0; j < width*size; j++){
// 		    drawPixel(x+j,y+i,mat[(i/size)*width+j/size]);
// 	    }
// 	}
// }
//
// void drawChar(uint64_t x, uint64_t y, uint8_t character, uint64_t fontSize, uint64_t fontColor, uint64_t backgroundColor){
// 	int aux_x = x;
// 	int aux_y = y;
//
//
// 	char bitIsPresent;
//
// 	unsigned char* toDraw = charBitmap(character);
//
// 	for (int i = 0; i < CHAR_HEIGHT; i++){
// 		for (int j = 0; j < CHAR_WIDTH; j++){
// 			bitIsPresent = (1<< (CHAR_WIDTH-j)) & toDraw[i];
//
// 			if (bitIsPresent){
// 				drawSquare(aux_x,aux_y, fontSize, fontColor);
// 			} else {
// 				drawSquare(aux_x, aux_y, fontSize, backgroundColor);
// 			}
// 			aux_x+=fontSize;
// 		}
// 		aux_x = x;
// 		aux_y+= fontSize;
// 	}
// }
//
// void drawString(int x,int  y, char *string ,int fontSize,int fontColor, int backgroundColor){
// 	if (x>screenData->width || y > screenData->height || x < 0 || y < 0){
// 		return ;
// 	}
//
// 	for (int i = 0, move=0; string[i]; i++,move +=((CHAR_SPACE+CHAR_WIDTH))*fontSize) {
// 		if (move>screenData->width-CHAR_WIDTH*fontSize)
// 		{
// 			y += (CHAR_SPACE+CHAR_HEIGHT)*fontSize ;
// 			move = 0;
// 		}
// 		if (y > screenData->height - CHAR_HEIGHT*fontSize){
// 			return;
// 		}
// 		drawChar(x+move,y,string[i],fontSize,fontColor,backgroundColor);
// 	}
// }
//
// void drawPixel(uint64_t x, uint64_t y, uint64_t color ) {
// 		if (x>screenData->width || y > screenData->height || x < 0 || y < 0){
// 			return ;
// 		}
//     char * curpos = screenData->framebuffer;
//     curpos+=(x+screenData->width*y)*3;
//
//     uint8_t b = color & 0x0000FF;
//     uint8_t g = (color >> 8 )& 0x0000FF;
//     uint8_t r = ( color >> 16) & 0x0000FF;
//
//     *curpos = b;
//     curpos++;
//     *curpos = g;
//     curpos++;
//     *curpos = r;
//     curpos++;
// }
//
// uint64_t getCharWidth() {
// 	return CHAR_WIDTH;
// }
// #endif

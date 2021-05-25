// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef STD_GRAPHICS
#define STD_GRAPHICS
#include <syscallsASM.h>
#include <stdint.h>

//NOTA: Hay un bug en el case en el que x = 0, no se porque no lo dibuja
//TODO: Arreglar eso

void drawString(int x, int y, char * buffer, int dim, uint64_t fontColor, uint64_t backgroundColor, int fontSize, int alphaBackground) {
  drawStringSysCall(buffer, (uint64_t)dim, (uint64_t)x, (uint64_t)y, fontColor, backgroundColor, (uint64_t)fontSize, (uint64_t)alphaBackground);
}

void clearScreen(uint64_t color) {
  clearDisplaySyscall(color);
}

void drawRect(int xi, int yi, int width, int height, uint64_t color){
  drawRectSyscall((uint64_t) xi, (uint64_t) yi, (uint64_t) width, (uint64_t) height, color);
}

void drawMatrix(int x, int y, int *mat,int width, int height,int size){
  drawMatrixSyscall((uint64_t) x,(uint64_t) y,(uint64_t *)mat,(uint64_t) width,(uint64_t) height,(uint64_t) size);
}
#endif

#ifndef FUNCIONES_PARA_TESTEO_H
#define FUNCIONES_PARA_TESTEO_H

#include <video_driver.h>
#include <keyboard_driver.h>
#include <timer_driver.h>
#include <regi.h>
#include <stdint.h>

void printReg();
void intToStr(uint64_t num, uint64_t strDim, char str[strDim]);
void printStr(uint64_t x, uint64_t y, char * str, uint64_t size);

void printDate() {

}

void printReg() {
  registerStruct * registers = getRegisters();
  uint64_t * pos = (uint64_t *) registers;
  uint64_t y = 0;
  char buffer[32] = {0};
  for (uint8_t i = 0; i < TOTAL_REGISTERS; i++) {
    intToStr(pos[i], 32, buffer);
    printStr(0, y, buffer, 32);
    y+=getCharHeight();
  }
}

void printStr(uint64_t x, uint64_t y, char * str, uint64_t size) {
  uint64_t xOffset = 0;
  for (uint64_t i = 0; i < size && str[i] != 0; i++) {
    drawChar(x+xOffset, y, str[i], 1, 0xFFFFFF, 0, 0);
    xOffset += getCharWidth();
  }
}

void intToStr(uint64_t num, uint64_t strDim, char str[strDim]) {
  if (num == 0) {
    str[0] = '0';
    str[1] = 0;
    return;
  }
  uint64_t index = 0;
  while (num > 0) {
    uint64_t rem = num % 10;
    for (uint64_t i = index; i > 0; i--) {
      str[i] = str[i-1];
    }
    str[0] = rem + '0';
    num/=10;
    index++;
  }
  str[index] = 0;
}

void printRes() {
  char buffer[8] = {0};
  uint64_t xOffset = 0;
  intToStr(getScreenWidthRes(), 8, buffer);
  for (uint64_t i = 0; i < 8 && buffer[i] != 0; i++) {
    drawChar(xOffset, 0, buffer[i], 1, 0xFFFFFF, 0, 1);
    xOffset += getCharWidth();
  }

  xOffset = 0;
  intToStr(getScreenHeightRes(), 8, buffer);
  for (uint64_t i = 0; i < 8 && buffer[i] != 0; i++) {
    drawChar(xOffset, 16, buffer[i], 1, 0xFFFFFF, 0, 1);
    xOffset += getCharWidth();
  }

}


#endif

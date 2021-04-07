#ifndef SYSCALLS
#define SYSCALLS
#include <stdint.h>
#include <regi.h>
#include <keyboard_driver.h>
#include <video_driver.h>
#include <timer_driver.h>
#include <date_driver.h>
#include <IO_driver.h>
#include <exceptions.h>

void writeStr(registerStruct * registers);
void getDateInfo(uint8_t mode, uint8_t * target);

void syscallHandler(registerStruct * registers) {
  uint64_t option = registers->rax;
  switch(option) {
    //READ KEYBOARD
    case 0:
    //rdi -> puntero a buffer
    //rsi -> uint8_t size
    //rdx -> putero a uint64_t count
    readKeyboard((char *)registers->rdi, (uint8_t) registers->rsi, (uint64_t*)registers->rdx);
    break;

    //WRITE STR
    case 1:
    //rdi -> puntero a buffer
    //rsi -> size
    //rdx -> x
    //rcx -> y
    //r8 -> fontColor
    //r9 -> backgroundColor
    //r10 -> fontSize
    //r11 -> alphaBackground
    writeStr(registers);
    break;

    case 2:
    //rdi color
      clearDisplay((uint64_t) registers->rdi);
      break;
    case 3:
    // rdi xstart , rsi ystart, rdx xend, rcx yend, r8 color
      drawLine((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8);
      break;
    case 4:
    // r9 xi, r8 yi, rsi color
      drawPixel((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx);
      break;
    case 5:
    // rdi xi, rsi yi , rdx width, rc8 height , r8 color
      drawRectangle((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8);
      break;
    case 6:
    // rdi xi, rsi yi, rdx puntero a matriz, rcx width, r8 height , r9 size
      drawMatrix((uint64_t) registers->rdi,(uint64_t) registers->rsi,(uint64_t *) registers->rdx,(uint64_t) registers->rcx,(uint64_t) registers->r8,(uint64_t) registers->r9);
      break;
    case 7:
      * ((uint64_t *)registers->rdi) = getTicks();
      break;
    case 8:
    //rdi -> mode
    //rsi -> puntero a entero
    getDateInfo((uint8_t) registers->rdi, (uint8_t *) registers->rsi);
    break;

    case 9: //Obtener los registros
    //rdi -> puntero a vector de uint64_t para guardar los valores
    getRegisters((uint64_t*)registers->rdi);
    break;

    case 10:
    //rdi -> direccion de la que se desea leer
    //rsi -> direccion del buffer para guardar
    get32bytesFromAddress(registers->rdi, (uint64_t*)registers->rsi, 1);
    break;

    case 11:
    //rdi -> puntero a int para devolver si hay algo
    bufferEmpty((uint64_t *) registers->rdi);
    break;

    case 12: //readError
    //rdi -> puntero a int para que devuelva el Error
    readError((uint64_t*)registers->rdi);
    break;
  }
}

void getDateInfo(uint8_t mode, uint8_t * target) {
  switch(mode) {
    case 0: *target = getSeconds();
    break;
    case 1: *target = getMinutes();
    break;
    case 2: *target = getHours();
    break;
    case 3: *target = getDay();
    break;
    case 4: *target = getMonth();
    break;
    case 5: *target = getYear();
    break;
  }
}

void writeStr(registerStruct * registers) {
  uint64_t xOffset = 0;
  char * buffer = (char *)registers->rdi;
  for (uint64_t i = 0; i < registers->rsi && buffer[i] != 0; i++) {
    char ch = ((char *)registers->rdi)[i];
    drawChar(registers->rdx + xOffset, registers->rcx, ch, registers->r10, registers->r8, registers->r9, registers->r11);
    xOffset += getCharWidth() * registers->r10;
  }
  //drawChar(0, 0, 'A',1, 0xFFFFFF, 0, 0);
}

#endif

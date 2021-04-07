#ifndef STD_IO
#define STD_IO

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <syscallsASM.h>


#include <stdGraphics.h>

#define PRINTF_BUFFER_SIZE 128

char std_in[STD_BUFFER_SIZE] = {0};
char std_out[STD_BUFFER_SIZE] = {0};
static char std_io_initialized = 0;
static char buffered_std_out = 1;

void stdio_init() {
  if (!std_io_initialized) { //para no inicializarlo 2 veces
    std_io_initialized = 1;
  }
}

char * getSTD_INAddress() {
  return std_in;
}

char * getSTD_OUTAddress() {
  return std_out;
}

int scanf(char * fmt, ...);

int printf(char * fmt, ...) {
  va_list varList;
  int i = 0;
  int j = 0;
  char printfBuff[PRINTF_BUFFER_SIZE] = {0};
  char tmpBuffer[32];

  while (fmt && fmt[i]) {
    printfBuff[j] = fmt[i];
    i++;
    j++;
  }
  //strcopy
  int index = 0;
  for (int index = 0; printfBuff[j] != 0 && index < j && index < PRINTF_BUFFER_SIZE; index++) {
    std_out[index] = printfBuff[j];
  }
  std_out[index] = 0;
  if (index != 0) {
    buffered_std_out = 1;
  }
}

void writeInStream(char stream, char * str, int size) {
  char * buffer;
  if (stream == 0) buffer = std_in;
  else if (stream == 1) buffer = std_out;
  else return;

  int i;
  for (i = 0; i < size && i < (STD_BUFFER_SIZE-1) && str[i] != 0; i++) {
    buffer[i] = str[i];
  }
  buffer[i] = 0;
}

int readKeyboard(char * buffer, int size) {
  if (size == 0) return 0;
  uint64_t aux;
  isKeyboardEmptySyscall(&aux);
  if (aux) {
    readKeyboardSysCall(buffer, (uint8_t) size);
    return 1;
  }
  return 0;
}

char somethingToPrint() {
  if (buffered_std_out) {
    buffered_std_out = 0;
    return 1;
  }
  return 0;
}





#endif

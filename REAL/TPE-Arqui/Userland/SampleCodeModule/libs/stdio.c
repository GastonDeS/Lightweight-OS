#ifndef STD_IO
#define STD_IO

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <syscallsASM.h>
#include <stdlib.h>
#include <stdGraphics.h>


void (*updateConsolePointer)(char *, int);
uint8_t updateConsoleInitialized = 0;

void scan(char * buff) {
  int ch = 0;
  int index = 0;
  while (ch != '\n') {
    if (ch)
      buff[index++] = ch;
      putChar(ch);
      ch = getChar();
  }
  putChar('\n');
}

void print(char * str, ...) {
  va_list vl;
  va_start(vl, str);
  char * auxPtr;
  char buffer[128] = {0};
  char tmp[20];
  int i = 0, j = 0;
  while (str && str[i]) {
    if (str[i] == '%') {
      i++;
      switch(str[i]) {
        case 'c':
        buffer[j++] = (char)va_arg( vl, int );
        break;
        case 'd':
        intToString(va_arg( vl, int ), tmp);
        strcpy(&buffer[j], tmp);
        j+=strlen(tmp);
        break;
        case 's':
        auxPtr = va_arg(vl, char*);
        strcpy(&buffer[j],auxPtr);
        j+=strlen(auxPtr);
        break;
        case 'x':
        intToBase(va_arg( vl, int ),16, tmp);
        strcpy(&buffer[j], tmp);
        j+=strlen(tmp);
        break;
        case 'X': //long hexa
        intToBase(va_arg( vl, uint64_t ),16, tmp);
        strcpy(&buffer[j], tmp);
        j+=strlen(tmp);
        break;
      }
    } else {
      buffer[j++] = str[i];
    }
    i++;
  }
  updateConsolePointer(buffer, j);
}

void putChar(char ch) {
  updateConsolePointer(&ch, 1);
}

int readKeyboard(char * buffer, int size) {
  if (size == 0) return 0;
  uint64_t aux;
  isKeyboardEmptySyscall(&aux);
  uint64_t count = 0;
  if (aux) {
    readKeyboardSysCall(buffer, (uint8_t) size, &count);
    return 1;
  }
  return 0;
}

void setConsoleUpdateFunction(void (*f)(char *, int)) {
  updateConsolePointer = f;
  updateConsoleInitialized = 1;
}

char getChar() {
  char ch = 0;
  uint64_t count;
  while(ch == 0 || count == 0) {
    readKeyboardSysCall(&ch, 1, &count);
  }
  return ch;
}

uint64_t getError() {
  uint64_t err = 32;
  readErrorSyscall(&err);
  return err;
}

#endif

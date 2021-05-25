// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef STD_IO
#define STD_IO

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <syscallsASM.h>
#include <stdlib.h>
#include <stdGraphics.h>
#include <pipe.h>


void (*updateConsolePointer)(char *, int);
uint8_t updateConsoleInitialized = 0;

void scan(int pipeR, int pipeW, char * buff) {
  int ch = 0;
  int index = 0;
  while (ch != '\n') {
    if (ch != 0){
      buff[index++] = ch;
      putChar(pipeW, ch);
      ch = getChar(pipeR);
    }
  }
  putChar(pipeW, '\n');
}

void print(int pipeW, char * str, ...) {
  va_list vl;
  va_start(vl, str);
  char * auxPtr;
  char buffer[1024*5] = {0};
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
  if (pipeW == -1) {
    updateConsolePointer(buffer, j);
  }else {
    pipeWrite(pipeW, buffer, j);
  }
}

void putChar(int pipeW,char ch) {
  if (pipeW == -1) {
    if (ch != (char)EOF)
      updateConsolePointer(&ch, 1);
  }else {
    pipeWrite(pipeW, &ch, 1);
  }
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

char getChar(int pipeR) {
  char ch = 0;
  uint64_t count=0;
  if (pipeR == -1) {
    while(ch == 0 || count == 0) {
    readKeyboardSysCall(&ch, 1, &count);
    }
  }else {
    while(ch == '\0' ) {
      pipeRead(pipeR, &ch, 1); //esto esta mal
    }
  }
  return ch;
}

uint64_t getError() {
  uint64_t err = 32;
  readErrorSyscall(&err);
  return err;
}

#endif

#ifndef STD_IO_H
#define STD_IO_H value

#define STD_BUFFER_SIZE 256

#include <stdarg.h>
#include <stdint.h>

int readKeyboard(char * buffer, int size);
void setConsoleUpdateFunction(void (*f)(char *, int));
char getChar();
void print(int pipeW, char *, ...);
void putChar(int pipeW, char ch);
void scan(int pipeR, int pipeW, char * buff);
uint64_t getError();
#endif

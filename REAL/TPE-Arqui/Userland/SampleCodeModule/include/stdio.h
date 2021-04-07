#ifndef STD_IO_H
#define STD_IO_H value

#define STD_BUFFER_SIZE 256

#include <stdarg.h>
#include <stdint.h>

int readKeyboard(char * buffer, int size);
void setConsoleUpdateFunction(void (*f)(char *, int));
char getChar();
void print(char *, ...);
void putChar(char ch);
void scan(char *);
uint64_t getError();
#endif

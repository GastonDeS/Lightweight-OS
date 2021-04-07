#ifndef STD_IO_H
#define STD_IO_H value

#define STD_BUFFER_SIZE 256

#include <stdarg.h>

void stdio_init();
char * getSTD_INAddress();
char * getSTD_OUTAddress();
int scanf(char * fmt, ...);
int printf(char * fmt, ...);
void writeInStream(char stream, char * str, int size);
int readKeyboard(char * buffer, int size);
char somethingToPrint();
#endif

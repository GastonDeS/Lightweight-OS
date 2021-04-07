#ifndef SYSCALLS_ASM_H
#define SYSCALLS_ASM_H

#include <stdint.h>

void drawStringSysCall(char * buff, uint64_t size, uint64_t x, uint64_t y, uint64_t fontColor, uint64_t backgroundColor, uint64_t fontSize, uint64_t alphaBackground);
void isKeyboardEmptySyscall(uint64_t * target);
void readKeyboardSysCall(char * buffer, uint64_t size);
void clearDisplaySyscall(uint64_t color);
void drawRectSyscall(uint64_t xi, uint64_t yi, uint64_t width, uint64_t height, uint64_t color);


#endif

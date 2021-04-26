#ifndef SYSCALLS_ASM_H
#define SYSCALLS_ASM_H

#include <stdint.h>

void drawStringSysCall(char * buff, uint64_t size, uint64_t x, uint64_t y, uint64_t fontColor, uint64_t backgroundColor, uint64_t fontSize, uint64_t alphaBackground);
void isKeyboardEmptySyscall(uint64_t * target);
void readKeyboardSysCall(char * buffer, uint64_t size, uint64_t * count);
void clearDisplaySyscall(uint64_t color);
void drawRectSyscall(uint64_t xi, uint64_t yi, uint64_t width, uint64_t height, uint64_t color);
void getTimeSyscall(uint8_t mode, uint8_t * ans);
void getRegistersSyscall(uint64_t * registers);
void drawMatrixSyscall(uint64_t x, uint64_t y, uint64_t *mat, uint64_t width, uint64_t height,uint64_t size);
void getMemSyscall(uint64_t, uint64_t*);
void readErrorSyscall(uint64_t *);
void getTicksSyscall(uint64_t *);
void sbrkSyscall(uint64_t, void *);
void createProcessSyscall(void (*)(),char **);
void endProcessSyscall(uint64_t pid);

#endif

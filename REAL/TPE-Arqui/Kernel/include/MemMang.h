#ifndef MEMMANG_H
#define MEMMANG_H

#include <stddef.h>
#include <memDrive.h>
#include <lib.h>
#include <checkMemdata.h>
#include <stdlib.h>

#define INFO_BLOCK_SIZE sizeof(struct infoBlock)
#define MAX_DIFF_SIZE 200 //Si un bloque supera esta diferencia entre su tamaño y el dato a guardar se lo divide
#define MIN_BYTES_REQUEST 500 //Cantidad minima de bytes pedidas al SO 

void* malloc(uint64_t size);
void free(void *ptr);
void *realloc(void *ptr, uint64_t newSize);

void mallocSyscall(uint64_t size, void** result);
void reallocSyscall(void *ptr, uint64_t newSize, void** result);

//debugger
void checkMemory(struct checkMemdata* data);
void printMem(char *str, int strSize);

#endif
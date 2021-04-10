#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define INFO_BLOCK_SIZE sizeof(struct infoBlock)

void MM_end();
void* my_malloc(size_t size);
void my_free(void *ptr);

//debugger
void printMemoryBLock(void);
void printAllMemory(void); 
void checkMemory(void);

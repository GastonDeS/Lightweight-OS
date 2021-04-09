#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 8192
#define INFO_BLOCK_SIZE sizeof(struct infoBlock)

void MM_end();
void* my_malloc(size_t size);
void free(void *ptr);

//debugger
void printMemoryBLock(void);
void printAllMemory(void); 

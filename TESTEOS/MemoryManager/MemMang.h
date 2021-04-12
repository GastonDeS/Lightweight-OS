#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define INFO_BLOCK_SIZE sizeof(struct infoBlock)
#define MAX_DIFF_SIZE 200 //Si un bloque supera esta diferencia entre su tamaño y el dato a guardar se lo divide
#define MIN_BYTES_REQUEST 500 //Cantidad minima de bytes pedidas al SO 

void MM_end();
void* my_malloc(size_t size);
void my_free(void *ptr);

//debugger
void printMemoryBLock(void);
void printAllMemory(void); 
int checkMemory(void);

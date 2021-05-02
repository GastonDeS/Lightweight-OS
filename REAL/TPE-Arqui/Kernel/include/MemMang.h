#ifndef MEMMANG
#define MEMMANG

#include <stdint.h>
#include <stddef.h>

#define INFO_BLOCK_SIZE sizeof(struct infoBlock)
#define MAX_DIFF_SIZE 200 //Si un bloque supera esta diferencia entre su tamaño y el dato a guardar se lo divide
#define MIN_BYTES_REQUEST 500 //Cantidad minima de bytes pedidas al SO 

void* malloc(uint64_t size);
void free(void *ptr);

#endif
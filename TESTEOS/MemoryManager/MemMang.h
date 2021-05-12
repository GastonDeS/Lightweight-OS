#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INFO_BLOCK_SIZE sizeof(struct infoBlock)
#define MAX_DIFF_SIZE 200 //Si un bloque supera esta diferencia entre su tamaño y el tamaño pedido por el usuario, se lo divide
#define MIN_BYTES_REQUEST 500 //Cantidad minima de bytes pedidas al SO 
#define ALIGN 0x40 //64 en hexa

void MM_end();
void* my_malloc(size_t size);
//si no hay mas espacio devuelve null y deja ptr como estaba
void* my_realloc(void *ptr, size_t newSize);
void my_free(void *ptr);

//debugger
void printMemoryBLock(int printFalg);
int checkMemory(int ptintFlag);

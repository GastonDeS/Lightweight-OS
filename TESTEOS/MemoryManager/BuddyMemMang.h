#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct BUDDY_HEADER {
   struct BUDDY_HEADER* next;
   uint64_t level;
}BUDDY_HEADER;

#define BUDDY_HEADER_SIZE sizeof(BUDDY_HEADER)

#define HEAP_SIZE 16384 // 4Mb entran aprox 1024 4kb sirve masomenos 

#define BLOCKS_PER_LEVEL(level) (int64_t)(1<<(level))
#define SIZE_OF_BLOCKS_AT_LEVEL(level) (uint64_t)(1 << (MAX_BLOCK_SIZE_LOG2 - level))

#define MIN_BLOCK_SIZE_LOG2 5
#define MIN_BLOCK_SIZE ((uint64_t) 1<<MIN_BLOCK_SIZE_LOG2)

#define MAX_BLOCK_SIZE_LOG2 14
#define MAX_BLOCK_SIZE ((uint64_t)1<<MAX_BLOCK_SIZE_LOG2)


#define LEVELS (MAX_BLOCK_SIZE_LOG2 - MIN_BLOCK_SIZE_LOG2)+1

#define INDEX_OF_POINTER_IN_LEVEL(pointer,level,memory_start) ((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(level))

void *my_malloc(uint64_t size);
void *recursiveMalloc(uint64_t level);
void my_free(void *ptr);
void recursiveFree(void *header, uint64_t level);
void insertBlock(void *header, uint64_t level);
void removeBlock(void * header, uint64_t level);
void *removeHeadBlock(uint64_t level);
int64_t getLevel(uint64_t size);
uint64_t getBlockNumber(BUDDY_HEADER *header);
void initialize();
void MM_end();
int checkMemory();
void addOccupied(void *header, uint64_t level);
int8_t removeOccupied(void *header);
uint64_t remainingBytes();
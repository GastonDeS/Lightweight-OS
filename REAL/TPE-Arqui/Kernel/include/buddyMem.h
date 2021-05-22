#ifndef MEMMANG_H
#define MEMMANG_H

#include <MemMang.h>
#include <stddef.h>
#include <memDrive.h>
#include <lib.h>

#define HEAP_SIZE 1024*1024*4 

typedef struct BUDDY_HEADER {
   struct BUDDY_HEADER* next;
   uint64_t level;
}BUDDY_HEADER;

#define BUDDY_HEADER_SIZE sizeof(BUDDY_HEADER)

#define BLOCKS_PER_LEVEL(level) (int64_t)(1<<(level))
#define SIZE_OF_BLOCKS_AT_LEVEL(level) (uint64_t)(1 << (MAX_BLOCK_SIZE_LOG2 - level))

#define MIN_BLOCK_SIZE_LOG2 5
#define MIN_BLOCK_SIZE ((uint64_t) 1<<MIN_BLOCK_SIZE_LOG2)

#define MAX_BLOCK_SIZE_LOG2 22
#define MAX_BLOCK_SIZE ((uint64_t)1<<MAX_BLOCK_SIZE_LOG2)


#define LEVELS (MAX_BLOCK_SIZE_LOG2 - MIN_BLOCK_SIZE_LOG2)+1

#define INDEX_OF_POINTER_IN_LEVEL(pointer,level,memory_start) ((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(level))

void *recursiveMalloc(uint64_t level);
void recursiveFree(void *header, uint64_t level);
void insertBlock(void *header, uint64_t level);
void removeBlock(void * header, uint64_t level);
void *removeHeadBlock(uint64_t level);
int64_t getLevel(uint64_t size);
uint64_t getBlockNumber(BUDDY_HEADER *header);
void initialize();
void addOccupied(void *header, uint64_t level);
int8_t removeOccupied(void *header);
uint64_t remainingBytes();

#endif
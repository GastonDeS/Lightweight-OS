#include <stdint.h>
#include <memDrive.h>
#include <video_driver.h>

#define NULL (void *)0

#define HEAP_SIZE 1024*1014*4  // 4Mb entran aprox 1024 4kb sirve masomenos 
void *startMemory = (void *)0x600000;
void *currentMemoryLimit = (void *)0x600000;

int availableMemCheck(uint64_t size);

void sbrk(uint64_t size,void ** answer){ 
    if(availableMemCheck(size)){
        currentMemoryLimit+=size;
        *answer = currentMemoryLimit;
    } else {
        *answer =  NULL;
    }
}

int availableMemCheck(uint64_t size){
    return (currentMemoryLimit-startMemory+size) <= HEAP_SIZE;
}
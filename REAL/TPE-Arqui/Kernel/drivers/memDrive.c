#include <stdint.h>
#include <memDrive.h>
#include <video_driver.h>
#include <stddef.h>

#define HEAP_SIZE 1024*1014*4  // 4Mb entran aprox 1024 4kb sirve masomenos 

void *startMemory = (void *)0x600000;
void *currentMemoryLimit = (void *)0x600000;

int availableMemCheck(uint64_t size);

void* sbrk(uint64_t size){ 
    if(availableMemCheck(size)){
        currentMemoryLimit+=size;
        return currentMemoryLimit;
    } else {
        return  NULL;
    }
}

int availableMemCheck(uint64_t size){
    return (currentMemoryLimit-startMemory+size) <= HEAP_SIZE;
}

void sbrSyscall(uint64_t size, void* result){
    result = sbrk(size);
}
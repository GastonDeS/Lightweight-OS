// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memDrive.h>

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

void sbrSyscall(uint64_t size, void** result){
    (*result) = sbrk(size);
}

void* getCurrentMemoryLimit(){
    return currentMemoryLimit;
}

void* getStartMemory(){
    return startMemory;
}
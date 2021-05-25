#ifndef MEMDRIVE_H
#define MEMDRIVE_H

#include <stdint.h>
#include <video_driver.h>
#include <stddef.h>

#define HEAP_SIZE (1024*1024*4)  // 4Mb entran aprox 1024 4kb sirve masomenos 

void* sbrk(uint64_t size);
void sbrSyscall(uint64_t size, void** result);

void* getCurrentMemoryLimit();
void* getStartMemory();

#endif
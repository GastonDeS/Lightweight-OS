#include <unistdK.h>
#include <memDrive.h>

#define PAGESIZE 4*1024 // 4KB
#define NULL (void*) 0

uint64_t countArgv(char **argv);

void createProcess(void (*function)(),char **argv){
    void *answer;
    sbrk(PAGESIZE,&answer); //TODO pasar a kmalloc y luego en addProcess guardarlo
    answer+=PAGESIZE;
    uint64_t argc = countArgv(argv);
    uint64_t *SP = (uint64_t *) createProcessInt(function,answer,argc,argv);
    addProcess(SP);
}

uint64_t countArgv(char **argv){
    int counter=0;
    for ( counter = 0; argv[counter]!= NULL ; counter++){}
    return counter;
}
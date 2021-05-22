#include <unistdK.h>
#include <MemMang.h>

#define PAGESIZE 4*8*1024 // 4KB

uint64_t countArgv(char **argv);

void createProcess(void (*function)(),char **argv,uint64_t *pid){
    void *answer;
    answer = malloc(PAGESIZE);
    answer+=PAGESIZE;
    uint64_t argc = countArgv(argv);
    uint64_t *SP = (uint64_t *) createProcessInt(function,answer,argc,argv);
    addProcess(SP,argv[0], pid);
}

uint64_t countArgv(char **argv){
    int counter=0;
    for ( counter = 0; argv[counter]!= NULL ; counter++){}
    return counter;
}
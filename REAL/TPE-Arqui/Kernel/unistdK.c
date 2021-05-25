// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <unistdK.h>

#define PAGESIZE (4*8*1024) // 4KB

//private:
uint64_t countArgv(char **argv);
char** copyArgv(int argc, char **argv);


void createProcess(void (*function)(),int foreground,char **argv,uint64_t *pid){
    void *answer;
    answer = malloc(PAGESIZE);
    if (answer == NULL) return;
    answer+=PAGESIZE;
    uint64_t argc = countArgv(argv);
    char **newArgv = copyArgv(argc, argv);
    uint64_t *SP = (uint64_t *) createProcessInt(function,answer,argc,newArgv);
    addProcess(SP,newArgv[0], pid, answer - PAGESIZE,foreground, argc, newArgv);
}


//private----

char** copyArgv(int argc, char **argv){
    char **newArgv = malloc(sizeof(char*)*argc);
    if (newArgv==NULL) return NULL;
    for (int i = 0; i < argc; i++){
        newArgv[i] = strCopy(argv[i]);
    }
    return newArgv;
}

uint64_t countArgv(char **argv){
    int counter=0;
    for ( counter = 0; argv[counter]!= NULL ; counter++){}
    return counter;
}


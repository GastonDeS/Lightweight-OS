

#include <testPipe.h>

void escritor(int id) {
    int result;
    while (1) {
        pipeWriteSyscall(id,"hello world!",14,&result);
    }
}

void lector(int  argc, char **argv) {
    int id = atoi(argv[1]);
    char *pipeBuff = malloc(sizeof(char)*124);
    int result;
    while (1) {
        pipeReadSyscall(id,pipeBuff,10,&result);
        print(pipeBuff);
        print("\n");
        for (int i = 0; i < 100000000; i++) {}
        
    }
}

void testPipe() {
    int pipeID;
    pipeSyscall(&pipeID);
    char num[10];
    intToString(pipeID,num);
    char *args[2];
    args[0] = "lector";
    args[1] = num;
    args[2] = NULL;
    createProcess(lector,args);
    escritor(pipeID);
}
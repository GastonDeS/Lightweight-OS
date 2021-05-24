

#include <testPipe.h>

int N = 2;

void escritor(int argc, char** argv) {
    int id = atoi(argv[1]);
    int semId = sem_open(SEM_NAME,1);
    
    pipeCreateSyscall(id, NULL);

    int result;
    for(int i=0; i< N; i++){
        sem_wait(semId);
        pipeWriteSyscall(id,argv[2],10,&result);
        yieldSyscall();
        sem_post(semId);
        //for (int i = 0; i < 100000000; i++) {}
    }

    sem_close(semId);
    pipeCloseSyscall(id, &result);
    myExit();
}

void lector(int  argc, char **argv) {
    int id = atoi(argv[1]);
    char *pipeBuff = malloc(sizeof(char)*124);
    int result;
    pipeCreateSyscall(id, NULL);
    for(int i=0; i<6; i++){
        pipeReadSyscall(id,pipeBuff,15,&result);
        print(pipeBuff);
        print(" ");
        //for (int i = 0; i < 100000000; i++) {}
        
    }
    pipeCloseSyscall(id, &result);
    myExit();
}

void testPipe() {
    int pipeID = 0;
    char num[10];
    intToString(pipeID,num);
    char *args[4];
    args[0] = "lector";
    args[1] = num;
    args[2] = NULL;
    createProcess(lector,args);
    char *args1[4];
    args1[0] = "escritor";
    args1[1] = num;
    args1[2] = "escritor1";
    args1[3] = NULL;
    createProcess(escritor,args1);
    char *args2[4];
    args2[0] = "escritor";
    args2[1] = num;
    args2[2] = "escritor2";
    args2[3] = NULL;
    createProcess(escritor,args2);
    char *args3[4];
    args3[0] = "escritor";
    args3[1] = num;
    args3[2] = "escritor3";
    args3[3] = NULL;
    createProcess(escritor,args3);
    
    myExit();

}
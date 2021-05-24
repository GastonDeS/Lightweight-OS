

#include <testPipe.h>

int N = 10;

void escritor(int argc, char** argv) {
    int id = atoi(argv[1]);
    int semId = sem_open(SEM_NAME,1);
    pipeOpen(id);
    int result;
    for(int i=0; i< N; i++){
        sem_wait(semId);
        pipeWrite(id,argv[2],10);
        sem_post(semId);
        yieldSyscall();
    }
    sem_close(semId);
    if(pipeClose(id)){
        print("\n cerror el sem del pipe %s \n",argv[0] );
    }
    myExit();
}

void lector(int  argc, char **argv) {
    int id = atoi(argv[1]);
    char *pipeBuff = malloc(sizeof(char)*15);
    int result;
    pipeOpen(id);
    for(int i=0; i<N*3; i++){
        pipeRead(id,pipeBuff,15);
        print(pipeBuff);
        print(" ");
    }
    if(pipeClose(id)){
        print("\n cerror el sem del pipe %s \n",argv[0] );
    }
    //free(pipeBuff);
    myExit();
}

void testPipe() {
    int pipeID = pipeCreate();

    char num[10];
    intToString(pipeID,num);
    char *args[4];
    args[0] = "lector";
    args[1] = num;
    args[2] = NULL;
    createProcess(lector,0,args);
    char *args1[4];
    args1[0] = "escritor";
    args1[1] = num;
    args1[2] = "escritor1";
    args1[3] = NULL;
    createProcess(escritor,0,args1);
    char *args2[4];
    args2[0] = "escritor";
    args2[1] = num;
    args2[2] = "escritor2";
    args2[3] = NULL;
    createProcess(escritor,0,args2);
    char *args3[4];
    args3[0] = "escritor";
    args3[1] = num;
    args3[2] = "escritor3";
    args3[3] = NULL;
    createProcess(escritor,0,args3);

    myExit();
}

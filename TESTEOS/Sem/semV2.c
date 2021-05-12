#include <semV2.h>
#include <time.h> //solo en tester
#define BLOCK 20

listADT* semVec = NULL;
int semVecDim = 0;  //espacio de semVec
int signalCheck = 0; 



//private:
int randNum(int min, int max){ //solo en tester
    return (min + rand() % (max + 1 - min));
}

void createSem(int semId, int* returnValue){
    if(semVec == NULL){ //primer llamado
        semVec = malloc(sizeof(listADT)*BLOCK);
        if(semVec == NULL)
            *returnValue = -1;
        semVecDim += BLOCK;
    }
    
    if(semVecDim < semId){ 
        semVec = (int*) realloc(semVec, sizeof(int)*(semVecDim+BLOCK));
        //chequear 
        semVecDim += BLOCK;
    }

    //creo una cola de pids de los proceso que esperan por entra al shMem
    semVec[semId] = newList(sizeof(int),NULL);
}

void removeSem(int semId, int* returnValue){

    //chequeo que no tenga procesos bloqueados
    if(!isEmpty(semVec[semId])){
        *returnValue = 0;
        return;
    }

    freeList(semVec[semId]);
    *returnValue = 1;

    return;
}

void semSleep(int semId, int* returnValue){
    if(signalCheck){
        signalCheck = 0;
        return;
    }   

    //obtengo el pid del proceso actual
    int pid;
    pid = randNum(0, 5); //solo en tester
    //getPid(&pid);

    //lo agrego a la cola de espera para entrar al shMem
    *returnValue = addToTheEnd(semVec[semId], &pid);

    //bloqueo al proceso
    blockProcess(pid); 
    return;
}

void semWakeUp(int semId, int* returnValue){
    *returnValue = 0;
    void* check = pop(semVec[semId]);
    
    //checke si esta vacia la lista
    if(check == NULL){
        signalCheck = 1;
        return;
    }
    //lo despierto
    int pid = *((int*) check);    
    unlockProcess(pid);

    *returnValue = 1;
    return;
}


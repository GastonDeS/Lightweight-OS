#include <sem.h>

typedef struct elem{
    int value;
    int numProcess;
    char *name;
    listADT* blockedProcesses;
}elem;

elem* semVec = NULL;
int semVecSize = 0; //cantidad de elemntos
int signalCheck = 0; 

//private:
int findFreeSpace();
int sleepProcess(listADT blockedProcesses);
int wakeUpProcess(listADT blockedProcesses);

void createSem(char *semName, int initialValue, int* returnValue){
    if(semVec == NULL){ //primer llamado
        semVec = malloc(sizeof(elem)*BLOCK);
        if(semVec == NULL){
            *returnValue = -1;
            return;
        }
    }
    

    //busco el primer espacio libre o si ya se creeo el semaforo
    int semId = findFreeSpace(semName);
    if(!semVec[semId].name && strcmp(semVec[semId].name, semName) == 0 ){ 
        semVec[semId].numProcess ++; 
    }else{// se encotro un espacio libre
        semVec[semId].value = initialValue;
        semVec[semId].name = semName;
        semVec[semId].numProcess = 1;
        semVec[semId].blockedProcesses = newList(sizeof(int),NULL);
    }
    semVecSize++;
}

void removeSem(int semId, int* returnValue){
    if(semId < 0  || semId > semVecSize)
        return -1;

    //chequeo que sea el ultimo proceso usando el semaforo
    semVec[semId].numProcess --;
    if(semVec[semId].numProcess != 0)
        return 1;
    
    //chequeo que no tenga procesos bloqueados
    if(!isEmpty(semVec[semId].blockedProcesses)){
        *returnValue = 0;
        return;
    }

    semVec[semId].name = NULL;
    semVec[semId].numProcess = 0;
    semVec[semId].value = -1;
    freeList(semVec[semId].blockedProcesses);
    
    semVecSize--;
    *returnValue = 1;
    return;
}

void semSleep(int semId, int* returnValue){
    if(semId < 0  || semId > semVecSize){
        *returnValue = -1;
        return;
    }
    if(signalCheck){
        signalCheck = 0;
        *returnValue = 0;
        return;
    }   

     if(semVec[semId].value >0)
        _xadd(-1,&(semVec[semId].value)); 
    else{
        *returnValue = sleepProcess(semVec[semId].blockedProcesses);
        if(returnValue ==  -1) {//si hubo un error al intentar dormir el proceso 
            return;
        }
        //se despierta solo si alguien hace un post
         _xadd(-1,&(semVec[semId].value)); //semVec[semId]--; 
    }

}

void semWakeUp(int semId, int* returnValue){
    *returnValue = 0;
    if(semId < 0  || semId > semVecSize)
        return -1;
    
    _xadd(1,&(semVec[semId].value));

    *returnValue = wakeUpProcess(semVec[semId].blockedProcesses);
    return;
}

//private---------------
int sleepProcess(listADT blockedProcesses){
     //obtengo el pid del proceso actual
    uint64_t pid;
    getPid(&pid);

    //lo agrego a la cola de espera para entrar al shMem
    int result;
    result = addToTheEnd(blockedProcesses, &pid);

    //bloqueo al proceso
    blockProcess(pid); 
    return result;
}

int wakeUpProcess(listADT blockedProcesses){
    
    void* check = pop(blockedProcesses);
    
    //checke si esta vacia la lista
    if(check == NULL){
        signalCheck = 1;
        return 0;
    }
    //lo despierto
    int pid = *((int*) check);    
    unlockProcess(pid);

    return 1;
}



int findFreeSpace(char *str){
    int i;
    int firstFree = -1;
    int foundFlag = 0;
    for (i = 0; i < semVecSize && !foundFlag; i++){
        if(firstFree == -1 && semVec[i].value == -1 )
            firstFree = i;
        
        if(strcmp(semVec[i].name, str) == 0)
            foundFlag = 1; 
    }

    if(foundFlag)
        return i;
    //si no lo encontro foundFlag = 0
    if(firstFree == -1) 
        return i;
    
    return firstFree;
}

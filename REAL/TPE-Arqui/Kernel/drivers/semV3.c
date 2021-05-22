#include <sem.h>

typedef struct elem{
    int value;
    int numProcess;
    char *name;
    listADT blockedProcesses;
}elem;

elem semVec[100];
int semVecSize = 0; //cantidad de elemntos

//private:
int findfreeSpaces();
int sleepProcess(listADT blockedProcesses);
int wakeUpProcess(listADT blockedProcesses);


void createSem(char *semName, int initialValue, int* returnValue){
    // if(semVec == NULL){ //primer llamado
    //     semVec = malloc(sizeof(elem)*BLOCK);
    //     if(semVec == NULL){
    //         *returnValue = -1;
    //         return;
    //     }
    // }
    

    //busco el primer espacio libre o si ya se creeo el semaforo
    int semId = findfreeSpaces(semName);
    if(semVec[semId].name && strcmp(semVec[semId].name, semName) == 0 ){ 
        semVec[semId].numProcess ++; 
    }else{// se encotro un espacio libre
        semVec[semId].value = initialValue;
        semVec[semId].name = semName;
        semVec[semId].numProcess = 1;
        semVec[semId].blockedProcesses = newList(sizeof(int),NULL);
        semVecSize++;
    }
}

void removeSem(int semId, int* returnValue){
    
    if(semId < 0  || semId > semVecSize){
        *returnValue = -1; 
        return;
    }
    //chequeo que sea el ultimo proceso usando el semaforo
    semVec[semId].numProcess --;
    if(semVec[semId].numProcess != 0){
        *returnValue = 1; 
        return;
    }
    
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
    if(semVec[semId].value >0)
        _xadd(-1,&(semVec[semId].value)); 
    else{
        *returnValue = sleepProcess(semVec[semId].blockedProcesses);
        if(*returnValue ==  -1) {//si hubo un error al intentar dormir el proceso 
            return;
        }
        //se despierta solo si alguien hace un post
    }

}

void semWakeUp(int semId, int* returnValue){
    if(semId < 0  || semId > semVecSize){
        *returnValue = -1;
        return;
    }
    _xadd(1,&(semVec[semId].value));

    *returnValue = wakeUpProcess(semVec[semId].blockedProcesses);
    if (*returnValue) {
        _xadd(-1,&(semVec[semId].value)); //semVec[semId]--; 
    }
    
    return;
}

void printSem(char *str, int strSize){
    int i=0, j=0, buffDim=10;
    strSize--; //reservo el lugar del \n
    char *title = "\nname\tvalue\t#process\t#blockProcess\n";
    char auxBuf[buffDim];

    //armado del title
    strcat2(str, &i, strSize, title);

    for(j=0 ;j < semVecSize && i < strSize; j++){
    
        //nombre
        strcat2(str, &i, strSize, semVec[j].name);
        strcat2(str, &i, strSize, "\t\t\t\t");

        //valor del semaforo
        intToString(semVec[j].value, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        strcat2(str, &i, strSize, "\t\t\t\t\t\t\t");

        //numero de procesos usando el semaforo
        intToString(semVec[j].numProcess, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        strcat2(str, &i, strSize, "\t\t\t\t\t\t\t\t\t\t\t");

        //numero procesos bloqueados 
        intToString(size(semVec[j].blockedProcesses), auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        strcat2(str, &i, strSize, "\n");
    }
    str[i] = '\0'; 
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
    if (result ==0 ){
        int ans;
        blockProcess(pid, &ans); 
    }
    return result;
}

int wakeUpProcess(listADT blockedProcesses){
    
    void* check = pop(blockedProcesses);
    
    //checke si esta vacia la lista
    if(check == NULL){
        return 0;
    }
    //lo despierto
    int pid = *((int*) check);
    free(check);
    int ans;
    unlockProcess(pid, &ans);
    return 1;
}



int findfreeSpaces(char *str){
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
        return i-1;
    //si no lo encontro foundFlag = 0
    if(firstFree == -1) 
        return i;
    
    return firstFree;
}

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
int findFreeSpace();
int sleepProcess(listADT blockedProcesses);
int wakeUpProcess(listADT blockedProcesses);
int strcpy2(char *dest, int* freeSpace, char * src);
int delimiter(int *freeSpace, char *str, char* token);


void createSem(char *semName, int initialValue, int* returnValue){
    // if(semVec == NULL){ //primer llamado
    //     semVec = malloc(sizeof(elem)*BLOCK);
    //     if(semVec == NULL){
    //         *returnValue = -1;
    //         return;
    //     }
    // }
    

    //busco el primer espacio libre o si ya se creeo el semaforo
    int semId = findFreeSpace(semName);
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
        //  _xadd(-1,&(semVec[semId].value)); //semVec[semId]--; 
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

void printSem(char *str){
    int i=0, j=0, buffDim=10, strFreeSpace = 100;
    //char *str = malloc(sizeof(char*)*strFreeSpace+1); //+1 es por el \0
    //char* str = [strFreeSpace+1]; //+1 es por el \0
    char *title = "\nname\tvalue\t#process\t#blockProcess";
    char auxBuf[buffDim];

    //armado del title
    i += strcpy2(str+i, &strFreeSpace, title);
    i += delimiter(&strFreeSpace, str+i, "\n");
    if(i < 0){
        str[i++] = '\0';
        return;
    }

    while(j < semVecSize){
    
        //nombre
        i += strcpy2(str+i, &strFreeSpace, semVec[j].name);
        i += delimiter(&strFreeSpace, str+i, "\t" );

        if(i < 0){i++;break;}

        //valor del semaforo
        intToString(semVec[j].value, auxBuf);
        i += strcpy2(str+i, &strFreeSpace, auxBuf);
        i += delimiter(&strFreeSpace, str+i, "\t" );

        if(i < 0){i++;break;}

        //numero de procesos usando el semaforo
        intToString(semVec[j].numProcess, auxBuf);
        i += strcpy2(str+i, &strFreeSpace, auxBuf);
        i += delimiter(&strFreeSpace, str+i, "\t" );

        if(i < 0){i++;break;}

        //numero procesos bloqueados 
        intToString(size(semVec[j].blockedProcesses), auxBuf);
        i += strcpy2(str+i, &strFreeSpace, auxBuf);
        i += delimiter(&strFreeSpace, str+i, "\n" );

        if(i < 0){i++;break;}
        j++;
    }
    str[i] = '\0'; 
}


//private---------------
int strcpy2(char * dest, int* freeSpace, char * src) {
  int i;
  for (i = 0; src[i] != 0 && *freeSpace != 0; i++) {
    dest[i] = src[i];
    (*freeSpace)--;
  }
  return i;
}

int delimiter(int *freeSpace, char *str, char* token){
    int i = -1;    
    if(*freeSpace == 0 ){
        *str = '\0';
        return i;
    }
    for (i = 0; token[i] != 0 && *freeSpace != 0; i++) {
        str[i] = token[i];
        (*freeSpace)--;
   }
   return i;
}


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
        return i-1;
    //si no lo encontro foundFlag = 0
    if(firstFree == -1) 
        return i;
    
    return firstFree;
}

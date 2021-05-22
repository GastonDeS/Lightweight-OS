#include <pipeDriver.h>

typedef struct elem{
    char free;
    char data[PIPE_SIZE];
    int readIndex;
    int writeIndex;
    int numProcess;
    int semId;
    char processWantRead;
    char processWantsWrite;
}elem;

elem pipeVec[BLOCK];
int pipeVecSize = 0; //cantidad de elemntos


//private
int findSpaces();
char* creatSemName(int pipeId);
int sleepProcess2();
int wakeUpProcess2(int pid);

void pipe(int *returnValue){
    int pipeId;
    if((pipeId=findSpaces()) != 0){
        pipeVec[pipeId].numProcess++; //encontro uno
    }else if(pipeVecSize+1 < BLOCK){//no hay, usa uno nuevo
        pipeId = pipeVecSize;
        createSem(creatSemName(pipeId), 1, &pipeVec[pipeId].semId);
        if(pipeVec[pipeId].semId == -1){
            *returnValue =  -1;
            return;
        }
        //crear listas
        pipeVec[pipeId].processWantRead = 0;
        pipeVec[pipeId].processWantsWrite= 0; 
        //listas
        pipeVec[pipeId].free = 0;
        pipeVec[pipeId].readIndex = 0;
        pipeVec[pipeId].writeIndex = 0;
        pipeVec[pipeId].numProcess = 1;        
        pipeVecSize++;
        *returnValue =  pipeId;
        return;
    }
    *returnValue =  -1;
    return;
}


void pipeClose(int pipeId, int *returnValue){

    if((pipeVec[pipeId].numProcess--) == 0){
        pipeVec[pipeId].free = 1;
        removeSem(pipeVec[pipeId].semId, NULL); //chequear
        *returnValue = 1;
        return;
    }

    *returnValue = 0;
    return;
}

void pipeWrite(int pipeId, char *addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    pipeVec[pipeId].processWantsWrite = 1;
    semWait(pipeVec[pipeId].semId, returnValue);
    if(*returnValue == -1)
        return;
    pipeVec[pipeId].processWantsWrite = 0;
    
    for(int i = 0; i < n; i++){
        
        while(pipeVec[pipeId].writeIndex == pipeVec[pipeId].readIndex + PIPE_SIZE){
            if(pipeVec[pipeId].processWantRead){
                semPost(pipeVec[pipeId].semId, returnValue);
                pipeVec[pipeId].processWantsWrite = 1;
                semWait(pipeVec[pipeId].semId, returnValue);
                if(*returnValue == -1)
                    return;
                pipeVec[pipeId].processWantsWrite = 0;
            }else{
                return;
            }
        }
        pipeVec[pipeId].data[ pipeVec[pipeId].writeIndex++ % PIPE_SIZE ] = addr[i]; 
    }

    semPost(pipeVec[pipeId].semId, returnValue);
    return;
}

void pipeRead(int pipeId, char * addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    pipeVec[pipeId].processWantRead = 1;
    semWait(pipeVec[pipeId].semId, returnValue);
    if(*returnValue == -1)
        return;
    pipeVec[pipeId].processWantRead = 0;
    
    for(int i = 0; i < n; i++){
       while( pipeVec[pipeId].readIndex == pipeVec[pipeId].writeIndex){
            if(pipeVec[pipeId].processWantsWrite){
                semPost(pipeVec[pipeId].semId, returnValue);
                pipeVec[pipeId].processWantRead = 1;
                semWait(pipeVec[pipeId].semId, returnValue);
                pipeVec[pipeId].processWantRead = 0;
            }else{
                return;
            }
        }
        addr[i] = pipeVec[pipeId].data[ pipeVec[pipeId].readIndex++ % PIPE_SIZE ]; 
    }
    semPost(pipeVec[pipeId].semId, returnValue);
    return;
}


//private:
int findSpaces(){
    int i;
    for (i = 0; i < pipeVecSize; i++){
        if(pipeVec[i].free)
            return i;
    }
    return 0;
}

char* creatSemName(int pipeId){
    int i=0, strSize=10;
    char str = (char*) malloc(sizeof(char)*strSize);
    char auxBuff[2];
    char* title = "pipeSem";
    strcat2(str, &i, strSize, title);
    intToString(pipeId, auxBuff);
    strcat2(str, &i, strSize, auxBuff);
    return str;
} 

int sleepProcess2(){
     //obtengo el pid del proceso actual
    uint64_t pid;
    getPid(&pid);

    int ans;
    blockProcess(pid, &ans); 
    return 0;
}

int wakeUpProcess2(int pid){
    
    int ans;
    unlockProcess(pid, &ans);
    return 1;
}

/*
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
*/
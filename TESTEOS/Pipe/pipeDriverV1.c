#include <pipeDriver.h>

typedef struct elem{
    char free;
    char data[PIPE_SIZE];
    int readIndex;
    int writeIndex;

    int semLock;
    listADT processToRead;
    listADT processToWrite;
}elem;

elem pipeVec[BLOCK];
int pipeVecSize = 0; //cantidad de elemntos


//private
int findSpaces();
char* createSemName(int pipeId);
int addMeToList(listADT list);
int removeMeOfList(listADT list);
int popList(listADT list);

void pipe(int *returnValue){
    
    int pipeId = findSpaces();
    if(pipeId == -1 && pipeVecSize+1 < BLOCK ){
        pipeId = pipeVecSize; //uso un nuevo espacio
    }else{
        *returnValue = -1;
        return; //no hay mas epsacios
    }
    //creo el semaforo
    createSem(createSemName(pipeId), 1, &pipeVec[pipeId].semLock);
    if(pipeVec[pipeId].semLock == -1){
        *returnValue =  -1;
        return;
    }
    //listas
    pipeVec[pipeId].processToWrite = newList(sizeof(int),NULL);
    pipeVec[pipeId].processToRead = newList(sizeof(int),NULL);
        
    //lo demas
    pipeVec[pipeId].free = 0;
    pipeVec[pipeId].readIndex = 0;
    pipeVec[pipeId].writeIndex = 0;    
    //aumento en uno el size 
    pipeVecSize++;
    
    *returnValue =  pipeId;
    return;
}


void pipeClose(int pipeId, int *returnValue){

    pipeVec[pipeId].free = 1;
    removeSem(pipeVec[pipeId].semLock, NULL);

    freeList(pipeVec[pipeId].processToRead);
    freeList(pipeVec[pipeId].processToWrite);
    
    pipeVecSize--;
    *returnValue = 1;
    return;

    *returnValue = 1;
        return;
}

void pipeWrite(int pipeId, char *addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    addMeToList(pipeVec[pipeId].processToWrite);
    semWait(pipeVec[pipeId].semLock, returnValue);
    if(*returnValue == -1)
        return;
    removeMeOfList(pipeVec[pipeId].processToWrite);
    
    for(int i = 0; i < n; i++){
        
        while(pipeVec[pipeId].writeIndex == pipeVec[pipeId].readIndex + PIPE_SIZE){
            if(!isEmpty(pipeVec[pipeId].processToRead)){
                int readerPid = popList(pipeVec[pipeId].processToRead);
                semPostPid(pipeVec[pipeId].semLock, readerPid);

                addMeToList(pipeVec[pipeId].processToWrite);
                semWait(pipeVec[pipeId].semLock, returnValue);
                if(*returnValue == -1)
                    return;
                removeMeOfList(pipeVec[pipeId].processToWrite);

            }else{
                semPost(pipeVec[pipeId].semLock, returnValue);
                return;
            }
        }
        pipeVec[pipeId].data[ pipeVec[pipeId].writeIndex++ % PIPE_SIZE ] = addr[i]; 
    }

    semPost(pipeVec[pipeId].semLock, returnValue);
    return;
}

void pipeRead(int pipeId, char * addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    addMeToList(pipeVec[pipeId].processToRead);
    semWait(pipeVec[pipeId].semLock, returnValue);
    if(*returnValue == -1)
        return;
    removeMeOfList(pipeVec[pipeId].processToRead);
    
    for(int i = 0; i < n && pipeVec[pipeId].data[ pipeVec[pipeId].readIndex % PIPE_SIZE ]; i++){
       while( pipeVec[pipeId].readIndex == pipeVec[pipeId].writeIndex){
            if(!isEmpty(pipeVec[pipeId].processToWrite)){
                int readerPid = popList(pipeVec[pipeId].processToWrite);
                semPostPid(pipeVec[pipeId].semLock, readerPid);

                addMeToList(pipeVec[pipeId].processToRead);
                semWait(pipeVec[pipeId].semLock, returnValue);
                if(*returnValue == -1)
                    return;
                removeMeOfList(pipeVec[pipeId].processToRead);
            }else{
                semPost(pipeVec[pipeId].semLock, returnValue);
                return;
            }
        }
        addr[i] = pipeVec[pipeId].data[ pipeVec[pipeId].readIndex++ % PIPE_SIZE ]; 
    }
    semPost(pipeVec[pipeId].semLock, returnValue);
    return;
}


//private:
int findSpaces(){
    int i;
    for (i = 0; i < pipeVecSize; i++){
        if(pipeVec[i].free)
            return i;
    }
    return -1;
}

//title de no mas de 8 caracteres
char* createSemName(int pipeId){
    int i=0, strSize=10;
    char *str = (char*) malloc(sizeof(char)*strSize);
    char auxBuff[2];
    char* title = "pipeS:";
    strcat2(str, &i, strSize, title);
    intToString(pipeId, auxBuff);
    strcat2(str, &i, strSize, auxBuff);
    return str;
} 

int addMeToList(listADT list){
    //obtengo el pid del proceso actual
    uint64_t pid;
    getPid(&pid);

    int result;
    result = insert(list, &pid);
    return result;
}

int popList(listADT list) {
    void* check = pop(list);
    
    //checke si esta vacia la lista
    if(check == NULL){
        return 0;
    }
    //lo despierto
    int pid = *((int*) check);
    free(check);
    return pid;
}

int removeMeOfList(listADT list){
    uint64_t pid;
    getPid(&pid);
    
    int result;
    result = delete(list, &pid);
    
    return result;
}

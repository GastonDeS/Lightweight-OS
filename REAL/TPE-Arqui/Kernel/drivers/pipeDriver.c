#include <pipeDriver.h>

typedef struct elem{
    char free;
    char data[PIPE_SIZE];
    int readIndex;
    int writeIndex;
    int semLock;
    char processToRead;
    char processToWrite;
}elem;

elem pipeVec[BLOCK];
int pipeVecSize = 0; //cantidad de elemntos

//private
int findSpaces();
char* creatSemName(int pipeId);

void pipe(int *returnValue){
    int pipeId = findSpaces();
    if(pipeId == -1 && pipeVecSize+1 < BLOCK ){
        pipeId = pipeVecSize; //uso un nuevo espacio
    }else{
        *returnValue = -1;
        return; //no hay mas epsacios
    }
    pipeId = pipeVecSize;
    createSem(creatSemName(pipeId), 1, &pipeVec[pipeId].semLock);
    if(pipeVec[pipeId].semLock == -1){
        *returnValue =  -1;
        return;
    }
    pipeVec[pipeId].processToRead = 0;
    pipeVec[pipeId].processToWrite= 0; 
    pipeVec[pipeId].free = 0;
    pipeVec[pipeId].readIndex = 0;
    pipeVec[pipeId].writeIndex = 0;      
    pipeVecSize++;
    *returnValue =  pipeId;
    return;

}


void pipeClose(int pipeId, int *returnValue){
    //solo elimina el pipe si se puede eliminar el sem
    pipeVec[pipeId].free = 1;
    removeSem(pipeVec[pipeId].semLock, returnValue);
    if(*returnValue)//si se elimino el sem returnValue=1, sino =0
        pipeVecSize--;
    return;
}

void pipeWrite(int pipeId, char *addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    pipeVec[pipeId].processToWrite = 1;
    semWait(pipeVec[pipeId].semLock, returnValue);
    if(*returnValue == -1)
        return;
    pipeVec[pipeId].processToWrite = 0;
    
    for(int i = 0; i < n ; i++){
        
        while(pipeVec[pipeId].writeIndex == pipeVec[pipeId].readIndex + PIPE_SIZE){
            //if(pipeVec[pipeId].processToRead){
                semPost(pipeVec[pipeId].semLock, returnValue);
                pipeVec[pipeId].processToWrite = 1;
                semWait(pipeVec[pipeId].semLock, returnValue);
                if(*returnValue == -1)
                    return;
                pipeVec[pipeId].processToWrite = 0;
            //}
            //else{
            //    semPost(pipeVec[pipeId].semLock, returnValue);
            //    return;
            //}
        }
        pipeVec[pipeId].data[ pipeVec[pipeId].writeIndex++ % PIPE_SIZE ] = addr[i];
        if(addr[i] == '\0') break;
    }
    pipeVec[pipeId].data[ pipeVec[pipeId].writeIndex % PIPE_SIZE ] = 0;
    semPost(pipeVec[pipeId].semLock, returnValue);
    return;
}

void pipeRead(int pipeId, char * addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    pipeVec[pipeId].processToRead = 1;
    semWait(pipeVec[pipeId].semLock, returnValue);
    if(*returnValue == -1)
        return;
    pipeVec[pipeId].processToRead = 0;
    int i;
    for( i = 0; i < n ; i++){
       while( pipeVec[pipeId].readIndex == pipeVec[pipeId].writeIndex){
            //if(pipeVec[pipeId].processToWrite){
                semPost(pipeVec[pipeId].semLock, returnValue);
                pipeVec[pipeId].processToRead = 1;
                semWait(pipeVec[pipeId].semLock, returnValue);
                pipeVec[pipeId].processToRead = 0;
            //}
            //else{
            //    agrego a la lista de bloqueados para leer
            //    semPost(pipeVec[pipeId].semLock, returnValue);
            //          
            //    return;
            //}
        }
        addr[i] = pipeVec[pipeId].data[ pipeVec[pipeId].readIndex++ % PIPE_SIZE ]; 
        if( pipeVec[pipeId].data[ pipeVec[pipeId].readIndex % PIPE_SIZE ] == '\0') break;
    }
    if (i != n) pipeVec[pipeId].readIndex++;
    semPost(pipeVec[pipeId].semLock, returnValue);
    return;
}

void printPipe(char *str, int strSize){
    int i=0, j=0, buffDim=10, aux;
    strSize--; //reservo el lugar del \n
    char *title = "\npipeId  rIndex    WIndex    rBLock  wBlock\n";
    char auxBuf[buffDim];

    //armado del title
    strcat2(str, &i, strSize, title);

    for(j=0 ;j < pipeVecSize && i < strSize; j++){
    
        aux = intToString(j, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 8-aux);

        aux = intToString(pipeVec[j].readIndex, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 10-aux);

        aux = intToString(pipeVec[j].writeIndex, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 10-aux);

        if(pipeVec[j].processToRead)
           aux = strcat2(str, &i, strSize, "yes");
        else
            aux = strcat2(str, &i, strSize, "no");
        addSpace(str, &i, strSize, 8-aux);

        if(pipeVec[j].processToWrite)
           aux = strcat2(str, &i, strSize, "yes");
        else
            aux = strcat2(str, &i, strSize, "no");
        strcat2(str, &i, strSize, "\n ");
    }
    str[i] = '\0';
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

char* creatSemName(int pipeId){
    int i=0, strSize=10;
    char *str = (char*) malloc(sizeof(char)*strSize);
    char auxBuff[2];
    char* title = "semP:";
    strcat2(str, &i, strSize, title);
    intToString(pipeId, auxBuff);
    strcat2(str, &i, strSize, auxBuff);
    return str;
} 
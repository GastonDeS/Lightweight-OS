#include <pipe_driver.h>

typedef struct elem{
    char free;
    char data[PIPE_SIZE];
    int readIndex;
    int writeIndex;
    int numProcess;
    int semId;
}elem;

elem pipeVec[BLOCK];
int pipeVecSize = 0; //cantidad de elemntos

//private
int findSpaces();
char* creatSemName(pipeId);

void pipe(int *returnValue){
    int pipeId;

    if((pipeId=findSpaces()) != 0){//busco espacio libre
        pipeVec[pipeId].numProcess++; //encontro uno
    }else if(pipeVecSize+1 < BLOCK){//no hay, usa uno nuevo
        pipeId = pipeVecSize;
        createSem(creatSemName(pipeId), 1, pipeVec[pipeId].semId);
        if(pipeVec[pipeId].semId == -1){
            *returnValue =  -1;
            return;
        }
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
    int pipeId;
    if( (pipeId = findSpaces()) != 0){//busco el pipe
        *returnValue = 1; //no lo encontro
        return;
    }

    if((pipeVec[pipeId].numProcess--) == 0){
        pipeVec[pipeId].free = 1;
        removeSem(pipeVec[pipeId].semId, NULL); //chequear
        *returnValue = 1;
        return;
    }

    *returnValue = 0;
    return;
}

void pipeWrite(int pipeId, char * addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    semSleep(pipeVec[pipeId].semId, returnValue);
    if(*returnValue == -1)
        return;
    
    for(int i = 0; i < n; i++){
        
        //falta algo

        pipeVec[pipeId].data[ pipeVec[pipeId].writeIndex % PIPE_SIZE ] = addr[i]; 
    }

    semWakeUp(pipeVec[pipeId].semId, returnValue);
    if(*returnValue == -1)
        return;
}

void pipeRead(int pipeId, char * addr, int n, int *returnValue){
    if(pipeId < 0  || pipeId > pipeVecSize){
        *returnValue = -1;
        return;
    } 

    semSleep(pipeVec[pipeId].semId, returnValue);

    //falta algo

    if(*returnValue == -1)
        return;
    
    for(int i = 0; i < n; i++){
        if( pipeVec[pipeId].readIndex == pipeVec[pipeId].readIndex)
            break;
        pipeVec[pipeId].data[ pipeVec[pipeId].writeIndex % PIPE_SIZE ] = addr[i]; 
    }

    semWakeUp(pipeVec[pipeId].semId, returnValue);
    if(*returnValue == -1)
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

char* creatSemName(pipeId){
    int i=0, strSize=10;
    char str[strSize];
    char auxBuff[2];
    char* title = "pipeSem";
    strcat2(str, &i, strSize, title);
    intToString(pipeId, auxBuff);
    strcat2(str, &i, strSize, auxBuff);
    return str;
} 
  
#include <pipeDriver.h>

#define IN_USE 1
#define FREE 0

typedef struct elem{
    char state;
    char data[BUFF_SIZE];
    int readIndex;
    int writeIndex;
    char processToRead;
    char processToWrite;
    int lockS;
    int numProcess;
}elem;

elem pipe[PIPE_MAX];
int numOfPipe = 0; //cantidad de elemntos


//private
int newPipe(int pipeId);
int findPipe(int pipeId);
int findFreePipe(int pipeId);
char* createSemName(int pipeId);


void pipeCreate(int pipeId, int *returnValue){
    if(findPipe(pipeId)){
        pipe[pipeId].numProcess++;
        *returnValue = pipeId;
        return;
    }

    pipeId = findFreePipe(pipeId);
    if(pipeId == -1){
        *returnValue = -1;
        return;
    }

    if(!newPipe(pipeId)){
        *returnValue = -1;
    }

    *returnValue =  pipeId;
    return;
}



void pipeClose(int pipeId, int *returnValue){

    if(!findPipe(pipeId)){
        *returnValue = -1;
        return;
    }
    pipe[pipeId].numProcess--;

    if(pipe[pipeId].numProcess > 0){
        *returnValue = 0;
        return;
    }

    removeSem(pipe[pipeId].lockS, returnValue);
    pipe[pipeId].state = FREE;
    numOfPipe--;
    return;

}


void pipeWrite(int pipeId, char *addr, int n, int *returnValue){
    if(!findPipe(pipeId)){
        *returnValue = -1;
        return;
    }

    pipe[pipeId].processToWrite = 1;
    semWait(pipe[pipeId].lockS, returnValue);
    pipe[pipeId].processToWrite = 0;

    for(int i = 0; i < n ; i++){
        while(pipe[pipeId].writeIndex == pipe[pipeId].readIndex + BUFF_SIZE){
            semPost(pipe[pipeId].lockS, returnValue);
            pipe[pipeId].processToWrite = 1;
            yield();
            semWait(pipe[pipeId].lockS, returnValue);
            pipe[pipeId].processToWrite = 0;
        }
        pipe[pipeId].data[ pipe[pipeId].writeIndex++ % BUFF_SIZE ] = addr[i];
        if(addr[i] == '\0') break;
    }
    semPost(pipe[pipeId].lockS, returnValue);
    return;
}

void pipeRead(int pipeId, char * addr, int n, int *returnValue){
    if(!findPipe(pipeId)){
        *returnValue = -1;
        return;
    }

    pipe[pipeId].processToRead = 1;
    semWait(pipe[pipeId].lockS, returnValue);
    pipe[pipeId].processToRead = 0;

    int i;
    for( i = 0; i < n ; i++){
       while( pipe[pipeId].readIndex == pipe[pipeId].writeIndex){
                semPost(pipe[pipeId].lockS, returnValue);
                pipe[pipeId].processToRead = 1;
                yield();
                semWait(pipe[pipeId].lockS, returnValue);
                pipe[pipeId].processToRead = 0;
        }
        addr[i] = pipe[pipeId].data[ pipe[pipeId].readIndex++ % BUFF_SIZE ];
        if( addr[i] == '\0') break;
    }
    // if (i != n) pipe[pipeId].readIndex++;
    semPost(pipe[pipeId].lockS, returnValue);
    return;
}


void printPipe(char *str, int strSize){
    int i=0, j=0, k=0, buffDim=10, aux;
    strSize--; //reservo el lugar del \n
    char *title = "\npipeId  rIndex    WIndex    rBLock  wBlock\n";
    char auxBuf[buffDim];

    //armado del title
    strcat2(str, &i, strSize, title);

    for(j=0 ;j < numOfPipe && i < strSize; j++, k++){

        //salteo los pipes libres
        while(k < PIPE_MAX && pipe[k].state == FREE){k++;}

        aux = intToString(k, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 8-aux);

        aux = intToString(pipe[k].readIndex, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 10-aux);

        aux = intToString(pipe[k].writeIndex, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 10-aux);

        if(pipe[k].processToRead)
           aux = strcat2(str, &i, strSize, "yes");
        else
            aux = strcat2(str, &i, strSize, "no");
        addSpace(str, &i, strSize, 8-aux);

        if(pipe[k].processToWrite)
           aux = strcat2(str, &i, strSize, "yes");
        else
            aux = strcat2(str, &i, strSize, "no");
        strcat2(str, &i, strSize, "\n ");
    }
    str[i] = '\0';
}


//private:
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

int newPipe(int pipeId){
    createSem(createSemName(pipeId), 1, &pipe[pipeId].lockS);
    if(pipe[pipeId].lockS == -1)
        return 0;

    pipe[pipeId].processToRead = 0;
    pipe[pipeId].processToWrite= 0;
    pipe[pipeId].state = IN_USE;
    pipe[pipeId].readIndex = 0;
    pipe[pipeId].writeIndex = 0;
    numOfPipe++;
    return 1;
}

int findPipe(int pipeId){
    return pipe[pipeId].state == IN_USE;
}

int findFreePipe(int pipeId){
    for (int i = 0; i < PIPE_MAX; i++){
        if(pipe[i].state == FREE)
            return i;
    }
    return -1;
}

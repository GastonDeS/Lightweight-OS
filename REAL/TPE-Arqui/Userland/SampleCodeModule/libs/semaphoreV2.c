#include <mySemaphore.h>

#define BLOCK 10000

typedef struct elem{
    int value;
    int numProcess;
    char *name;
}elem;

int lock = 1;

elem* semVec = NULL;
int semVecSize = 0; //cantidad de elemntos
int semVecDim = 0;  //espacio de semVec


//private:
int findFreeSpace();
void reallocVec();
void lock_post();
void lock_wait();


int sem_open(char* name, int initialValue){
    lock_wait();

    if(initialValue < 0)
        return -1;
    
    if(semVec == NULL){ //la primera vez creo el vector
        semVec = malloc(sizeof(elem)*BLOCK);
        if(semVec == NULL){
            return -1;
        }
        semVecDim += BLOCK;
    }

    //busco el primer espacio libre
    int semId = findFreeSpace(name);
    if(!semVec[semId].name && strcmp(semVec[semId].name, name) == 0){ //encontro un espacio libre
        semVec[semId].numProcess ++;
    }else{
        semVec[semId].value = initialValue;
        semVec[semId].name = name;
        semVec[semId].numProcess = 1;
    }
    semVecSize++;

    //creo el sem en el kernel
    int returnValue;  
    createSemSyscall(semId, &returnValue); 
    if(returnValue == -1)
        return -1;

    lock_post();
    return semId;
}

//elimina el semforo solo si no tiene procesos eperando
//si se libero devulve 1 sino 0 y si hubo un error -1
int sem_close(int semId){
    lock_wait();
    if(semId < 0  || semId > semVecSize)
        return -1;
    
    semVec[semId].numProcess --;
    if(semVec[semId].numProcess != 0)
        return 0;

    //remuevo el sem del kernel
    int returnValue = 0;
    removeSemSyscall(semId, &returnValue);
    
    //libero el lugar solo si se elimino el sem en el kernel
    if(returnValue) {
        semVecSize--;
        semVec[semId].value = -1; 
    }
    lock_post();
    return returnValue;
}

int sem_wait(int semId){
    if(semId < 0  || semId > semVecSize)
        return -1;
    
    lock_wait();
    if(semVec[semId].value >0)
        _xadd(-1,&(semVec[semId].value)); 
    else{
        int returnValue;
        semSleepSyscall(semId, &returnValue);
        if(returnValue ==  -1) {//si hubo un error al intentar dormir el proceso 
            lock_post();
            return returnValue;
        }
        //se despierta solo si alguien hace un post
         _xadd(-1,&(semVec[semId].value)); //semVec[semId]--; 
    }
    lock_post();
    return 1;
}

int sem_post(int semId){
    if(semId < 0  || semId > semVecSize)
        return -1;
    lock_wait();
    _xadd(1,&(semVec[semId].value));//semVec[semId] ++;

    int returnValue;
    semWakeUpSyscall(semId, &returnValue);
    lock_post();
    return returnValue;
}

//private---------------
void reallocVec(){
    /*
    semVec = realloc(semVec, sizeof(elem)*(semVecDim+BLOCK));
    //chequear si hubo error
    semVecDim += BLOCK;
    */
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
    //i--;

    if(foundFlag == 0 && firstFree == -1 && semVecSize == semVecDim) 
        reallocVec();

    if(foundFlag)
        return i;
    //si no lo encontro foundFlag = 0
    if(firstFree == -1) 
        return i;
    
    return firstFree;
}


void lock_wait(){
    while(_xadd(-1, &lock) <= 0)
        _xadd(1, &lock);
}       

void lock_post(){
     _xadd(1, &lock);
}
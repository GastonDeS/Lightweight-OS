#include <semUserlandV2.h>
#include <semAsm.h>

#define BLOCK 20

int* semVec = NULL;
int semVecSize = 0; //cantidad de elemntos
int semVecDim = 0;  //espacio de semVec


//private:
int findFreeSpace();
void reallocVec();

/* falta: 
**  ->que se achique el vector a medida que se liberen los sem 
**  ->testear     
*/

int newSem(int initialValue){
    if(initialValue >= 0)
        return -1;
    
    if(semVec == NULL){ //la primera vez creo el vector
        semVec = (int*) malloc(sizeof(int)*BLOCK);
        if(semVec == NULL)
            return -1;
        semVecDim += BLOCK;
    }

    //busco el primer espacio libre
    int semId = findFreeSpace();
    semVec[semId] = initialValue;

    //creo el sem en el kernel
    int* returnValue ;  
    createSem(semId, &returnValue); 
    if(returnValue == -1)
        return -1;

    return semId;
}

//elimina el semforo solo si no tiene procesos eperando
//si se libero devulve 1 sino 0 y si hubo un error -1
int freeSem(int semId){
    if(semId < 0  || semId > semVecSize)
        return -1;

    //remuevo el sem del kernel
    int returnValue = 0;
    removeSem(semId, &returnValue);
    
    //libero el lugar solo si se elimino el sem en el kernel
    if(returnValue) 
        semVec[semId] = -1; 
    
    return returnValue;
}

int sem_wait(int semId){
    if(semId < 0  || semId > semVecSize)
        return -1;
    
    if(semVec[semId]>0)
        _xadd(-1,&semVec[semId]); //semVec[semId]--;
    else{
        int returnValue;
        semSleep(semId, &returnValue);
        if(returnValue ==  -1) //si hubo un error al intentar dormir el proceso 
            return returnValue;

        //se despierta solo si alguien hace un post
         _xadd(-1,&semVec[semId]); //semVec[semId]--; 
    }
    return 1;
}

int sem_post(int semId){
    if(semId < 0  || semId > semVecSize)
        return -1;

    _xadd(1,&semVec[semId]);//semVec[semId] ++;

    int returnValue;
    semWakeUp(semId, &returnValue);
    
    return returnValue;
}


//private:
void reallocVec(){
    semVec = (int*) realloc(semVec, sizeof(int)* (semVecDim+BLOCK));
    //chequear si hubo error
    semVecDim += BLOCK;
}

int findFreeSpace(){
    
    int i;
    for (i = 0; i < semVecSize; i++){
        if(semVec[i] == -1 )
            return i;
    }
    if(semVecSize == semVecDim) 
        reallocVec();

    semVecSize++;
    return i;
}
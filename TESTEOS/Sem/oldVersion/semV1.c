#include <sem.h>

int maxId = 0;

typedef struct{
    int semId;
    listADT processQueue;
}semData;

int equals(void *e1, void *e2){
    semData element1 = *((semData *)e1);
    semData element2 = *((semData *)e2);
    return element2.semId == element1.semId;
};

listADT semList = NULL;

//private:
int addToBlockList(int pid, int id);
int removeToBlockList(int semId);



void createSem(int* semId){
    if(semList == NULL)
        semList = newList(sizeof(semData), equals);

    semData newSem = {maxId++,NULL}; 
    newSem.processQueue = newList(sizeof(int),NULL);

    insert(semList, &newSem);    
    *semId = maxId;
}

void removeSem(int semId, int* returnValue){
    if(semId >= maxId)
        return;
    
    semData aux = {semId,NULL};
    aux = *((semData*)getElem(semList, &aux));
    
    /*//si el semaforo tenia los desbloqueo
    while(!isEmpty(aux.processQueue))
        unlockProcess( *((int*)next(aux.processQueue)) );
        eliminar proceso de la cola
    */

    //si el semaforo tiene proceso bloqueados no se puede eliminar
    if(!isEmpty(aux.processQueue)){
        *returnValue = 0;
        return;
    }

    deleteElem(semList, &aux, deleteElemValue);
    *returnValue = 1;
    return;
}

void semSleep(int semId, int* returnValue){
    if(semId >= maxId)
        return;

    int pid;
    getPid(&pid);

    *returnValue = addToBlockList(pid, semId);

    blockProcess(pid); 
    return;
}



void semWakeUp(int semId, int* returnValue){
    if(semId >= maxId)
        return;

    //busaco el pid el proceso a despertar
    int pid = removeToBlockList(semId);
    
    //chequeo que exista
    *returnValue = pid;
    if(pid == -1)
        return;
        
    //lo despierto
    unlockProcess(pid);
    return;
}




//privare:

void deleteElemValue(void* innerList){
    freeList((listADT)innerList);
}

int addToBlockList(int pid, int semId){
    //en aux esta el semData con el semId buscado
    semData aux = {semId, NULL};
    aux = *((semData*)getElem(semList, &aux));
    
    //cheque que exista un semData con el id buscado
    if(&aux == NULL)
        return -1;

    //agrego al final de la cola el pid del proceso que espea entrar al shMem
    return addToTheEnd(aux.processQueue, &pid);
    
}

int removeToBlockList(int semId){
    //en aux esta el semData con el semId buscado
    semData aux = {semId, NULL};
    aux = *((semData *)getElem(semList, &aux));

    //cheque que exista un semData con el id buscado
    if(&aux == NULL)
        return -1;

    //sacao de la cola de procesos bloqueados el proximo proceso a despertar
    int pid = *((int*) pop(aux.processQueue));

    return pid;
}

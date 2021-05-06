#include <sem.h>


//private:
int addToBlockList(int pid, int id);
int removeToBlockList(int semId);

int createSem(){

    if(semList == NULL)
        semList = newList(sizeof(semData), equals);

    semData newSem = {maxId++,NULL}; 
    newSem.processQueue = newList(sizeof(int),NULL);

    insert(semList, &newSem);    
    return maxId;
}

void semSleep(int semId, int* flag){
    int pid;
    getPid(&pid);

    *flag = addToBlockList(pid, semId);

    blockProcess(pid); 
    return;
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

void semWakeUp(int semId, int* flag){

    //busaco el pid el proceso a despertar
    int pid = removeToBlockList(semId);
    
    //chequeo que exista
    *flag = pid;
    if(pid == -1)
        return;
        
    //lo despierto
    unlockProcess(pid);
    return;
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

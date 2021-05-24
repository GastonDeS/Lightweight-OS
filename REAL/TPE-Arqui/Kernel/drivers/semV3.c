#include <sem.h>

#define IN_USE 1
#define FREE 0

typedef struct elem{
    char state;
    int value;
    int numProcess;
    char *name;
    listADT blockedProcesses;
}elem;

elem sem[SEM_MAX]; 
int numOfSem = 0;  //cantidad de semaforos

int equalsSem(void* n1, void* n2){
    int aux1 = *((int*)n1);
    int aux2 = *((int*)n2);
    return aux1 == aux2;
}

//private:
int findSem(char *semName);
int isSem(int semId);
int findFreeSem();
void newSem(int semId, char* semName, int initialValue);
int sleepProcess(listADT blockedProcesses);
int wakeUpProcess(listADT blockedProcesses);


void createSem(char *semName, int initialValue, int* returnValue){
    if(semName == NULL){
        *returnValue = -1;
        return;
    }

    int semId = findSem(semName);
    
    if(semId != -1)
        sem[semId].numProcess++;
    else{
        semId = findFreeSem();
        if(semId == -1){
            *returnValue = -1;
            return;
        }
        newSem(semId, semName, initialValue);
    }   

    *returnValue = semId;
    return;
}

void removeSem(int semId, int* returnValue){
    if(!isSem(semId)){
        *returnValue = -1; 
        return;
    }

    //chequeo que sea el ultimo proceso usando el semaforo
    sem[semId].numProcess --;
    if(sem[semId].numProcess != 0){
        *returnValue = 1; 
        return;
    }
    
    //chequeo que no tenga procesos bloqueados
    if(!isEmpty(sem[semId].blockedProcesses)){
        while( wakeUpProcess(sem[semId].blockedProcesses) != 0);
        *returnValue = 0;
        return;
    }

    free(sem[semId].name);
    freeList(sem[semId].blockedProcesses);
    sem[semId].name = NULL;
    sem[semId].numProcess = 0;
    sem[semId].value = 0;
    sem[semId].state = FREE;
    numOfSem--;

    *returnValue = 1;
    return;
}

void semWait(int semId, int* returnValue){
    if(!isSem(semId)){
        *returnValue = -1; 
        return;
    }

    if(sem[semId].value > 0) {
        _xadd(-1,&(sem[semId].value)); 
        (*returnValue) = 0;
    }
    else{
        *returnValue = sleepProcess(sem[semId].blockedProcesses);
        if(*returnValue ==  -1) {//si hubo un error al intentar dormir el proceso 
            return;
        }
        //se despierta solo si alguien hace un post
    }
}

void semPost(int semId, int* returnValue){
    if(!isSem(semId)){
        *returnValue = -1; 
        return;
    }

    _xadd(1,&(sem[semId].value));

    *returnValue = wakeUpProcess(sem[semId].blockedProcesses);
    if (*returnValue) {
        _xadd(-1,&(sem[semId].value)); 
    }
    
    return;   
}

void printSem(char *str, int strSize){
    int i=0, j=0, k=0, buffDim=10, aux;
    strSize--; //reservo el lugar del \n
    char *title = "\nname         value  #process  #blockProcess\n";
    char auxBuf[buffDim];

    //armado del title
    strcat2(str, &i, strSize, title);

    for(j=0 ;j < numOfSem && i < strSize; j++, k++){
        
        //salteo los semaforos libres
        while(k < SEM_MAX && sem[k].state == FREE){k++;}

        //nombre
        aux = strlen(sem[k].name);
        strcat2(str, &i, strSize, sem[k].name);
        addSpace(str, &i, strSize, 13-aux);

        //valor del semaforo
        aux = intToString(sem[k].value, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 7-aux);

        //numero de procesos usando el semaforo
        aux = intToString(sem[k].numProcess, auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        addSpace(str, &i, strSize, 10-aux);

        //numero procesos bloqueados 
        aux = intToString(size(sem[k].blockedProcesses), auxBuf);
        strcat2(str, &i, strSize, auxBuf);
        strcat2(str, &i, strSize, "\n");
    }
    str[i] = '\0'; 
}

int semPidBlock(int semId){
    if(!isSem(semId))
        return -1; 

    return sleepProcess(sem[semId].blockedProcesses);
}

int semPostPid(int semId, int pid){
    if(!isSem(semId))
        return -1;

        
    _xadd(1,&(sem[semId].value));
    
    if(delete(sem[semId].blockedProcesses, &pid)){
        int ans;
        unlockProcess(pid, &ans);
        _xadd(-1,&(sem[semId].value));
        return ans;
    }
    return -1;
}



//private---------------
int findSem(char *semName){
    for(int i = 0; i < SEM_MAX; i++)
        if(sem[i].state == IN_USE && strcmp(sem[i].name, semName) == 0)
                return i;
    return -1;
}

int isSem(int semId){
    return sem[semId].state == IN_USE;
}

int findFreeSem(){
    for (int i = 0; i < SEM_MAX; i++)
        if(sem[i].state == FREE)
            return i;
    
    return -1;
}

void newSem(int semId, char* semName, int initialValue){

    sem[semId].state = IN_USE;
    sem[semId].value = initialValue;
    sem[semId].name = strCopy(semName);
    sem[semId].numProcess = 1;
    sem[semId].blockedProcesses = newList(sizeof(int),equalsSem);
    numOfSem++;
}

int sleepProcess(listADT blockedProcesses){
     //obtengo el pid del proceso actual
    uint64_t pid;
    getPid(&pid);

    //lo agrego a la cola de espera para entrar al shMem
    int result;
    result = addToTheEnd(blockedProcesses, &pid);

    //bloqueo al proceso
    if (result == 0 ){
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
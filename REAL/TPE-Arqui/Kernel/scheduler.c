#include <scheduler.h>
#include <listADT.h> 
#include <stddef.h>
#include <stdlib.h>

//#define MAXPROCESS 100
//#define PROCESS_SIZE 


typedef enum {ERROR, BLOCKED, KILLED, READY}State;

typedef struct {
    uint64_t *BP;
    uint64_t *SP;
    uint64_t *EP; // End Pointer 
    uint64_t pid;
    State state;
    char *name;
    uint8_t foreground;
    uint8_t priority;
}process;

int equals(void* n1, void* n2){
    process aux1 = *((process*)n1);
    process aux2 = *((process*)n2);
    return aux1.pid == aux2.pid;
}

listADT processList = NULL;

process *current = NULL;

int currentCountdownPriority;

//private:
void changeProcess();
int changeState(uint64_t pid , State state);
void addTitle(char *dest,int *j);
void strcat(char *dest, char *src, int *j);
void movIndex(char *dest,int to, int *from);


uint64_t * scheduler(uint64_t *currentProces){
    if (processList == NULL)
        return currentProces;
    
    if (current==NULL) {
        current = (process *)next(processList);
        return (*current).SP;
    }
    
    (*current).SP = currentProces;

    changeProcess();

    return (*current).SP;
}

void changeProcess(){
    if (currentCountdownPriority && current->state == READY) {
        currentCountdownPriority--;
    } else {
        current = (process *)next(processList);
        while ((*current).state!=READY) {
            current = (process *)next(processList);
        }
        currentCountdownPriority = current->priority;
    }
}

void nice(uint64_t pid, uint64_t priority, int *result){
    process *processAux = malloc(sizeof(process));
    process *toFree = processAux;
    (*processAux).pid = pid;
    processAux = (process*)getElem(processList, processAux);
    if (processAux!=NULL) {
        processAux->priority = priority; 
        (*result) = priority; 
        free(toFree);
        return; 
    }  
    free(toFree);
    (*result) = -1;
}

void yield(){
    currentCountdownPriority = 0;
    _hlt();
}

void addProcess(uint64_t *currentProces, char *name,uint64_t *pid,uint64_t *ep) {
    if(processList == NULL){
        processList = newList(sizeof(process),equals);
        if(processList == NULL)
            return;
    }
    process newProcess;
    newProcess.SP = currentProces;
    newProcess.pid = size(processList);
    newProcess.state = READY;
    newProcess.foreground = 0;
    newProcess.BP = currentProces;
    newProcess.EP = ep;
    newProcess.name = name;
    newProcess.priority = 3; 
    
    insertBeforeNext(processList, &newProcess);
    (*pid) = newProcess.pid;
    return;
}

void exceptionProcess(){
    freeList(processList);
    processList = NULL;
}

void freeEP(process * aux) {
    free(aux->EP);
}

void endProcessWrapper(uint64_t pid, int *result){
    if(pid > 0){
        process aux;
        aux.pid = pid;
        if (deleteElem(processList, &aux,freeEP)){
            (*result) = 0;
            if (current->pid == pid) _hlt();
            return;
        }
    }
    (*result) = -1;
}

void getPid(uint64_t *pid) {
    (*pid) =  (*current).pid;
    return;
}

void ps(char *result){
    process **arrayOfProcess = ListToArray(processList);
    int i = 0;
    int j = 0, k =0;
    char statesA[4][8] = {"Error","Blocked","Killed","Ready"}; 
    char title[41] = "pid   prio   state   SP     BP     name";
    strcat(result,title,&j);
    movIndex(result,49,&j);
    result[j++] = '\n';
    while (arrayOfProcess[i] != NULL) {
        result[j++] = ' ';
        char pid[3], prio[10],SP[10],BP[10];
        intToString(arrayOfProcess[i]->pid,pid);
        strcat(result,pid,&j);
        movIndex(result,6,&j);
        intToString(arrayOfProcess[i]->priority,prio);
        strcat(result,prio,&j);
        movIndex(result,13,&j);
        strcat(result,statesA[arrayOfProcess[i]->state],&j);
        movIndex(result,21,&j);
        intToBase(arrayOfProcess[i]->SP,16,SP);
        strcat(result,SP,&j);
        movIndex(result,28,&j);
        intToBase(arrayOfProcess[i]->BP,16,BP);
        strcat(result,BP,&j);
        movIndex(result,35,&j);
        strcat(result,arrayOfProcess[i]->name,&j);
        movIndex(result,49,&j);
        result[j++] = '\n';
        i++;
    }
    result[j] = 0;
    free(arrayOfProcess);
}

void movIndex(char *dest,int to, int *from){
    for ( ; (*from)%50 < to; (*from)++) {
        dest[(*from)] = ' ';
    }
}

void strcat(char *dest, char *src, int *j) {
    int k = 0;
    for ( k = 0 ; src[k] ;  (*j)++, k++ ) {
        dest[*j] = src[k];
    }
}

void blockProcess(uint64_t pid, int *result){
    // if (pid==0) return;
    if (changeState(pid, BLOCKED)) {
        (*result) = 0;
        if (current->pid == pid) _hlt();
        return;
    }
    (*result) = -1;
}

void unlockProcess(uint64_t pid, int *result){
    if (changeState(pid, READY)) {
        (*result) = 0;
        return;
    }
    (*result) = -1;
}

int changeState(uint64_t pid , State state){
    process *processAux = malloc(sizeof(process));
    process *toFree = processAux;
    (*processAux).pid = pid;
    processAux = (process*)getElem(processList, processAux);
    if (processAux!=NULL) {
        (*processAux).state = state;  
        free(toFree);
        return 1; 
    }
    free(toFree);
    return 0;
}




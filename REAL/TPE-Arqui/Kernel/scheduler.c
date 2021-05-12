#include <scheduler.h>
//#include <video_driver.h>
//#include <MemMang.h>
#include <listADT.h> 
#include <stddef.h>

//#define MAXPROCESS 100
//#define PROCESS_SIZE 


typedef enum {ERROR, BLOCKED, KILLED, READY}State;

typedef struct {
    uint64_t *SP;
    uint64_t pid;
    State state;
}process;



int equals(void* n1, void* n2){
    process aux1 = *((process*)n1);
    process aux2 = *((process*)n2);
    return aux1.pid == aux2.pid;
}

listADT processList = NULL;

process *current = NULL;

//private:
void changeProcess();


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
    current = (process *)next(processList);
    while ((*current).state==BLOCKED) {
        current = (process *)next(processList);
    }
    
}

void addProcess(uint64_t *currentProces) {
    if(processList == NULL){
        processList = newList(sizeof(process),equals);
        if(processList == NULL)
            return;
    }
    process newProcess;
    newProcess.SP = currentProces;
    newProcess.pid = size(processList);
    newProcess.state = READY;
    
    insertBeforeNext(processList, &newProcess);
    return;
}

void exceptionProcess(){
    freeList(processList);
    processList = NULL;
}

void endProcessWrapper(uint64_t pid){
    if(pid > 0){
        process aux;
        aux.pid = pid;
        delete(processList, &aux);
    }
}

void getPid(uint64_t *pid) {
    (*pid) =  (*current).pid;
    return;
}

void listAllProcess(char **ProcessList) {
    //*ProcessList = "PID\tForeground\tname\tSP\tBP\tPriority\t\n.0  \t1      \tshell\t0x60ffb0\t0x600000\t1";
}

void blockProcess(uint64_t pid){
    if (pid==0) return;
    changeState(pid, BLOCKED);
}

void unlockProcess(uint64_t pid){
    changeState(pid, READY);
}

void changeState(uint64_t pid , State state){
    process *processAux;
    (*processAux).pid = pid;
    processAux = (process*)getElem(processList, processAux);
    if (processAux!=NULL) {
        (*processAux).state = state;   
    }    
}




#include <scheduler.h>
#include <video_driver.h>
#include <MemMang.h>
#include <stddef.h>

#define MAXPROCESS 100
#define PROCESS_SIZE 

typedef enum {ERROR, BLOCKED, KILLED, READY}State;

typedef struct {
    uint64_t *SP;
    uint64_t times;
    uint64_t pid;
    State state;
}process;

process* processArray;
uint64_t processArrayDim = 0;
uint32_t load=0;
uint64_t currentPID=-1;

void changeProcess();
void addProcess(uint64_t *currentProces);
int isFirstTime();


uint64_t * scheduler(uint64_t *currentProces){
    if (load == 0) {
        return currentProces;
    }
    if ( currentPID!=-1) {
        processArray[currentPID].SP = currentProces;
    }
    changeProcess();

    return processArray[currentPID].SP;
}

int isFirstTime(){
    return processArray[currentPID].times==1;
}

void changeProcess(){
    currentPID  = (currentPID+1)%load;
    while( processArray[currentPID].state == BLOCKED)
        currentPID  = (currentPID+1)%load;
    return;
}

void addProcess(uint64_t *currentProces) {
    if (load == MAXPROCESS)
        return;
    if(load == 0){
        processArray = malloc(MAXPROCESS*100);
        if(processArray == NULL)
            return;
    }
    processArray[load].SP = currentProces;
    processArray[load].pid = load;
    processArray[load].times =0;
    processArray[load].state = READY;
    load++;
    return;
}


void exceptionProcess(){
    if(currentPID != 0){
        endProcess(processArray[currentPID].pid);
        return;
    }
    free(processArray);
    processArrayDim = 0;
    load=0;
    currentPID=-1;
}

void endProcessWrapper(uint64_t pid){
    if(pid > 0){
        if(processArray[currentPID].pid == pid)
            goToProcess(scheduler(currentPID));
        endProcess(pid);
    }
}

void endProcess(uint64_t pid) {
    int flag =1;
    for (int i = 0; i < load && flag; i++) {
        if (processArray[i].pid == pid) {
            flag = 0;
            for (int j= i+1; j < load+1; j++){
                processArray[i] = processArray[j];
                i++;
            }
            load--;
        }
    }
    return;
}

void getPid(uint64_t *pid) {
    (*pid) =  currentPID;
    return;
}

void listAllProcess(char **ProcessList) {
    *ProcessList = "PID\tForeground\tname\tSP\tBP\tPriority\t\n.0  \t1      \tshell\t0x60ffb0\t0x600000\t1";
}

void blockProcess(uint64_t pid){
    changeState(pid, BLOCKED);
}

void unlockProcess(uint64_t pid){
    changeState(pid, READY);
}

void changeState(uint64_t pid , State state){
     int flag = 1;
     if (pid !=0) {
        for (int i = 0; i < load && flag; i++) {
            if (processArray[i].pid == pid) {
                flag = 0;
                processArray[i].state  = state;
            }
        }
    }
}




#include <scheduler.h>
#include <video_driver.h>

#define MAXPROCESS 100

typedef enum {ERROR, BLOCKED, KILLED, READY}State;

typedef struct {
    uint64_t *SP;
    uint64_t times;
    uint64_t pid;
    State state;
}process;

process processArray[MAXPROCESS];
uint32_t load=0;
uint64_t currentPID=0;

void changeProcess();
void addProcess(uint64_t *currentProces);
int isFirstTime();


uint64_t * scheduler(uint64_t *currentProces){
    if (load == 0) {
        return currentProces;
    }
    processArray[currentPID].SP = currentProces;
    processArray[currentPID].times++;
    changeProcess();
    processArray[currentPID].times+=1;
    if (isFirstTime()) {
        goToFirstProcess(processArray[currentPID].SP);
    }

    return processArray[currentPID].SP;
}

int isFirstTime(){
    return processArray[currentPID].times==1;
}

void changeProcess(){
    currentPID  = (currentPID+1)%load;
    if(processArray[currentPID].state == BLOCKED)
        currentPID  = (currentPID+1)%load;
    return;
}

void addProcess(uint64_t *currentProces) {
    if (load == MAXPROCESS) {
        return ;
    } else {
        processArray[load].SP = currentProces;
        processArray[load].pid = load;
        processArray[load].times =0;
        load++;
        if (load==1) {
            processArray[load].times++;
            currentPID=load-1;
            goToFirstProcess(currentProces);
        }

    }
    return;
}

void endProcess(uint64_t pid) {
    int flag =1;
    if (pid !=0) {
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
                processArray->state  = state;
            }
        }
    }
}




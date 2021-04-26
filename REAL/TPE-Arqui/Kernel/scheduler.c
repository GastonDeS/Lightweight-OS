#include <scheduler.h>

#define MAXPROCESS 100

struct proces{
    uint64_t *SP;
    uint64_t PID;
};
typedef struct proces *process;

process processArray[MAXPROCESS] = {0};
uint8_t current=0;
uint8_t load=0;
uint64_t currentPID=0;

void changeProcess();
void addProcess(uint64_t *currentProces);


uint64_t * scheduler(uint64_t *currentProces){
    if (load == 0) {
        return &currentProces;
    }
    processArray[current]->SP = currentProces;
    changeProcess();
    return processArray[current]->SP;
}

void changeProcess(){
    current  = (current+1)%load;
    return;
}

void addProcess(uint64_t *currentProces) {
    if (load == MAXPROCESS) {
        return ;
    } else {
        processArray[load]->SP = currentProces;
        load++;
        processArray[load]->PID = currentPID++;
        if (load==1) {
            goToFirstProcess(currentProces);
        }
        
    }
    return;
}

void endProcess(uint64_t pid) {
    int flag =1;
    if (pid !=0) {
        for (int i = 0; i < MAXPROCESS && flag; i++) {
        if (processArray[i]->PID == pid) {
            flag = 0;
            for (int j= i+1; j < current; j++){
                processArray[i] = processArray[j];
                i++;
            }
            
        }
        
    }
}
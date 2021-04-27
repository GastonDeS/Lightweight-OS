#include <scheduler.h>
#include <video_driver.h>

#define MAXPROCESS 100

struct proces{
    uint64_t *SP;
    uint64_t pid;
};
typedef struct proces *process;

// process processArray[MAXPROCESS] = {0};
uint64_t *processArray[MAXPROCESS] = {0};
uint32_t current=0;
uint32_t load=0;

void changeProcess();
void addProcess(uint64_t *currentProces);


uint64_t * scheduler(uint64_t *currentProces){
    if (load == 0) {
        return currentProces;
    }
    int cur = current;
    uint64_t *prev = processArray[current];
    // = processArray[current]->SP;
    // processArray[current]->SP = currentProces;
    processArray[current] = currentProces;
    prev = processArray[current];
    changeProcess();
    cur = current;
    uint64_t * new = processArray[current];//->SP;
    return processArray[current];//->SP;
}

void changeProcess(){
    current  = (current+1)%load;
    return;
}

void addProcess(uint64_t *currentProces) {
    if (load == MAXPROCESS) {
        return ;
    } else {
        processArray[load]/*->SP*/ = currentProces;
        // processArray[load]/*->pid*/ = load;
        int cur = load;
        load++;
        cur = load;
        if (load==1) {
            current=load-1;
            goToFirstProcess(currentProces);
        }
        
    }
    return;
}
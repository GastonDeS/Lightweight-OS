#include <scheduler.h>
#include <video_driver.h>

#define MAXPROCESS 100

typedef struct {
    uint64_t *SP;
    uint64_t pid;
    uint64_t times;
}process;

process processArray[MAXPROCESS];
// uint64_t *processArray[MAXPROCESS] = {0};
uint32_t current=0;
uint32_t load=0;
uint64_t first=0;

void changeProcess();
void addProcess(uint64_t *currentProces);


uint64_t * scheduler(uint64_t *currentProces){
    if (load == 0) {
        return currentProces;
    }
    processArray[current].SP = currentProces;
    processArray[current].times++;
    changeProcess();
    if (processArray[current].times==0) {
        processArray[current].times+=1;
        goToFirstProcess(processArray[current].SP);
    }
    
    return processArray[current].SP;
}

void changeProcess(){
    current  = (current+1)%load;
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
            current=load-1;
            goToFirstProcess(currentProces);
        }
        
    }
    return;
}
#include <scheduler.h>
#include <video_driver.h>

#define MAXPROCESS 100

typedef struct {
    uint64_t *SP;
    uint64_t pid;
    uint64_t times;
}process;

process processArray[MAXPROCESS];
uint32_t current=0;
uint32_t load=0;
uint64_t first=0;

void changeProcess();
void addProcess(uint64_t *currentProces);
int isFirstTime();


uint64_t * scheduler(uint64_t *currentProces){
    if (load == 0) {
        return currentProces;
    }
    processArray[current].SP = currentProces;
    processArray[current].times++;
    changeProcess();
    processArray[current].times+=1;
    if (isFirstTime()) {
        goToFirstProcess(processArray[current].SP);
    }
    
    return processArray[current].SP;
}

int isFirstTime(){
    return processArray[current].times==1;
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
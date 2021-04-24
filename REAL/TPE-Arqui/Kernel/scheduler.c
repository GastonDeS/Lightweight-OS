#include <scheduler.h>

#define MAXPROCESS 10

struct proces{
    uint64_t *SP;
};
typedef struct proces *process;

process processArray[MAXPROCESS];
uint8_t current;
uint8_t load=0;

uint8_t isPresent(uint64_t *currentProces);
void changeProcess(uint64_t *currentProces);
void addProcess(uint64_t *currentProces);


uint64_t * scheduler(uint64_t *currentProces){
    if (!isPresent(currentProces)) {
        addProcess(currentProces);
    }
    changeProcess(currentProces);
    return processArray[current]->SP;
}

void changeProcess(uint64_t *currentProces){
    current = (current+1)%load;
    return;
}

uint8_t isPresent(uint64_t *currentProces){
    if (processArray[current]->SP == currentProces){
        return 1;
    }
    return 0;
}

void addProcess(uint64_t *currentProces) {
    if (load == MAXPROCESS) {
        return ;
    } else {
        processArray[load-1] = currentProces;
        load++;
    }
    return;
}
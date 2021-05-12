#include <stdint.h>
#include <stdlib.h>

#include "semTest.h" 
#include "CuTest.h"
#include "semUserlandV2.h"


void testsem(CuTest *const cuTest);

static const size_t TestQuantity = 1;
static const Test MemoryManagerTests[] = {testsem};


void inc(int R, int *p, int semId){
    for (int i = 0; i < R; i++){
        sem_wait(semId);
        (*p)++; 
        sem_post(semId);
    }
}

void testsem(CuTest *const cuTest){
    int global = 0;

    //crear n proseceso
    //que cada uno ejecute inc 
    
    //verificar que global = n*R

}


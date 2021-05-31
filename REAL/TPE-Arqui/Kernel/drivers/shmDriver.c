#include <shmDriver.h>

void *shm[SHM_SIZE];

int findShm(int shmId);

void shmOpen(int id, int size, void **returnValue){
    
    if(findShm(id)){
        (*returnValue) = shm[id];
        return;
    }

    shm[id] = malloc(sizeof(void*)*size);
    (*returnValue) = shm[id];
    return;
}


int findShm(int shmId){
    return shm[shmId] != NULL;
}

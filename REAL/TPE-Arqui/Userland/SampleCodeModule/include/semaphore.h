#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stddef.h>
#include <semAsm.h>
#include <stdlib.h> 
#include <syscallsASM.h>


//crea un nuevo semaforo
//se le pasa el valor inicial y devulve el id del semaforo
//si hubo un error devulve -1
int sem_open(char *name, int initialValue);

//elimina el semforo solo si no tiene procesos eperando
//si se libero devulve 1 sino 0 y si hubo un error -1
int sem_close(int semId);


//bloquea al prceso hasta que pueda acceder al shMem
//si hubo un error devulve -1 sino 1
int sem_wait(int semId);

//despierta al siguente proceso esperanso
//si habia procesos esperando devulve 1 sino 0
int sem_post(int semId);



#endif
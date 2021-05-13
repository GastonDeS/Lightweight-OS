/*#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

int my_create_process(char * name){
  return 0;
}

int my_sem_open(char *sem_id, int initialValue){
  int aux = newSem(initialValue);
  if(aux = -1)
    return aux;
  *sem_id = aux;
  return 0;
}

int my_sem_wait(char *sem_id){
  return sem_wait(*sem_id);
}

int my_sem_post(char *sem_id){
  return sem_post(*sem_id);
}

int my_sem_close(char *sem_id){
  return freeSem(*sem_id);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int global;  //shared memory

void slowInc(int *p, int inc){
  int aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(int sem, int value, int N){
  int i;

  if (sem && !my_sem_open(SEM_ID, 1)){
    printf("ERROR OPENING SEM\n");
    return;
  }
  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    slowInc(&global, value);
    if (sem) my_sem_post(SEM_ID);
  }

  if (sem) my_sem_close(SEM_ID);
  
  printf("Final value: %d\n", global);
}

void test_sync(){
  int i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 1, 1, 1000000);
    my_create_process("inc", 1, -1, 1000000);
  }
}

void test_no_sync(){
  int i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 0, 1, 1000000);
    my_create_process("inc", 0, -1, 1000000);
  }
}

int main(){
  test_sync();
  return 0;
}
*/
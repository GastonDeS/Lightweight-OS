#include <mySemaphore.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int global; //shared memory

//private:
int my_create_process(char *name, int sem, int value, int N);
void slowInc(int *p, int inc);
void inc(int sem, int value, int N);
void test_sync();
void test_no_sync();


int main(){
  test_sync();
  return 0;
}

int my_create_process(char *name, int sem, int value, int N){
  char **argv = {&sem, &value, &N};
  createProcess(inc, argv);
   return 0;
}

void slowInc(int *p, int inc){
  int aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(int sem, int value, int N){
  int i;

  int semId;
  if (sem && !(semId = sem_open(SEM_ID, 1))){
    printf("ERROR OPENING SEM\n");
    return;
  }

  for (i = 0; i < N; i++)
  {
    if (sem) sem_wait(semId);
    slowInc(&global, value);
    if (sem) sem_post(semId);
  }

  if (sem) sem_close(semId);

  printf("Final value: %d\n", global);
}

void test_sync(){
  int i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    my_create_process("inc", 1, 1, 1000000);
    my_create_process("inc", 1, -1, 1000000);
  }
}

void test_no_sync(){
  int i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    my_create_process("inc", 0, 1, 1000000);
    my_create_process("inc", 0, -1, 1000000);
  }

}

int main(){
  test_sync();
  return 0;
}


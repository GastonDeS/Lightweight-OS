#include <test_sync.h>

long global; //shared memory

//private:
int my_create_process(char *name, int sem, int value, int N);
void slowInc(int *p, int inc);
void inc(int sem, int value, int N);

int my_create_process(char *name, int sem, int value, int N){
  char **argv = {&sem, &value, &N};
  //argv[0] = &sem;
  //argv[1] = &value;
  //argv[2] = &N;
  createProcess(inc, NULL);
  return 0;
}

void slowInc(int *p, int inc){
  int aux = *p;
  aux += inc;
  //yield(); 
  *p = aux;
}

void inc(int sem, int value, int N){
  int i;
  int semId;

  sem = 0;
  value = 1;
  N = 1000;
  //print("\n %d : %d : %d \n", sem, value, N);

  if (sem && (semId = sem_open(SEM_ID, 1)) == -1){
    print("ERROR OPENING SEM\n");
    return;
  }
  for (i = 0; i < N; i++){
    if (sem) sem_wait(semId);
    slowInc(&global, value);
    if (sem) sem_post(semId);
  }
  if (sem) sem_close(semId);

  print("Final value: %d\n", global);
  myExit();
}

void test_sync(){
  int i;
  global = 0;

  print("\nCREATING PROCESSES...(WITH SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 1, 1, 1000000);
    my_create_process("inc", 1, -1, 1000000);
  }
  
}

void test_no_sync(){
  int i;
  global = 0;

  print("\nCREATING PROCESSES...(WITHOUT SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 0, 1, 1000000);
    my_create_process("inc", 0, -1, 1000000);
  }

}


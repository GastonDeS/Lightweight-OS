#include <test_sync.h>

long global; //shared memory
char *argv[5];

//private:
int my_create_process(char *name, int *sem, int *value, int *N);
void slowInc(int *p, int inc);
void inc(int argc, char **argv);


int my_create_process(char *name, int *sem, int *value, int *N){
  argv[0] = name;
  argv[1] = sem;
  argv[2] = value;
  argv[3] = N;
  argv[4] = NULL;

  createProcess(inc, argv);
  return 0;
}

void slowInc(int *p, int inc){
  int aux = *p;
  aux += inc;
  //yield(); 
  *p = aux;
}

void inc(int argc, char **argv){
  int i, semId;
  int sem = atoi(argv[1]);
  int value = atoi( argv[2] );
  int N = atoi(argv[3]);

  print("%d \n", N);

  
  if (sem && (semId = sem_open(SEM_ID, 1)) == -1){
    print("ERROR OPENING SEM\n");
    return;
  }
  for (i = 0; i < N; i++){
    if (sem) sem_wait(semId);
    global += value;
    //slowInc(&global, value);
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
    my_create_process("inc", "1", "1", "1000000");
    //my_create_process("inc", "1", "-1", "1000000");
  }
  
}

void test_no_sync(){
  int i;
  int sem = 0, suma = 1, resta = -1, N = 1000000;
  global = 0;

  print("\nCREATING PROCESSES...(WITH SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    my_create_process("inc", &sem, &suma, &N);
    my_create_process("inc", &sem, &resta, &N);
  }
}

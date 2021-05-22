#include <test_sync.h>
#include <commands.h>

int global; //shared memory
char *argv1[5];
char *argv2[5];

//private:
void slowInc(int *p, int inc);
void inc(int argc, char **argv);

void slowInc(int *p, int inc){
  int aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(int argc, char **argv){
  int i, semId;
  int sem = atoi(argv[1]);
  int value = atoi( argv[2] );
  int N = atoi(argv[3]);

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
    argv1[0] = "inc+";      //nombre del proceso
    argv1[1] = "1";         //1 si se quiere usar semaforos
    argv1[2] = "1";         //valor a sumar al shMem
    argv1[3] = "10";      //cantidad de entradas al shMem
    argv1[4] = NULL;
    createProcess(inc, argv1);
    argv2[0] = "inc-";
    argv2[1] = "1";
    argv2[2] = "-1";
    argv2[3] = "10";
    argv2[4] = NULL;
    createProcess(inc, argv2);
  }
}

void test_no_sync(){
  int i;
  global = 0;

  print("\nCREATING PROCESSES...(WITH SEM)\n");
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    argv1[0] = "inc+";
    argv1[1] = "0";
    argv1[2] = "1";
    argv1[3] = "10";
    argv1[4] = NULL;
    createProcess(inc, argv1);
    argv2[0] = "inc-";
    argv2[1] = "0";
    argv2[2] = "-1";
    argv2[3] = "10";
    argv2[4] = NULL;
    createProcess(inc, argv2);
  }
}

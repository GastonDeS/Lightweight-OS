// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_sync.h>


static int global; //shared memory
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
  int n = atoi(argv[3]);

  if (sem && (semId = sem_open(SEM_ID, 1)) == -1){
    print(STDOUT, "ERROR OPENING SEM\n");
    return;
  }
  for (i = 0; i < n; i++){
    if (sem) sem_wait(semId);
    slowInc(&global, value);
    if (sem) sem_post(semId);
  }
  if (sem) sem_close(semId);

  print(STDOUT, "Final value: %d\n", global);
  myExit();
}

void test_sync(int argc,char ** argv){
  int i;
  global = 0;

  print(STDOUT, "\nCREATING PROCESSES...(WITH SEM)\n");
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[5];
    argv1[0] = "inc+";      //nombre del proceso
    argv1[1] = "1";         //1 si se quiere usar semaforos
    argv1[2] = "1";         //valor a sumar al shMem
    argv1[3] = "1000";      //cantidad de entradas al shMem
    argv1[4] = NULL;
    createProcess(inc,0, argv1);
    argv1[0] = "inc-";
    argv1[1] = "1";
    argv1[2] = "-1";
    argv1[3] = "1000";
    argv1[4] = NULL;
    createProcess(inc,0, argv1);
  }
  myExit();
}

void test_no_sync(){
  int i;
  global = 0;

  print(STDOUT, "\nCREATING PROCESSES...(WITHOUT SEM)\n");
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[5];
    argv1[0] = "inc+";
    argv1[1] = "0";
    argv1[2] = "1";
    argv1[3] = "1000";
    argv1[4] = NULL;
    createProcess(inc,0, argv1);
    argv1[0] = "inc-";
    argv1[1] = "0";
    argv1[2] = "-1";
    argv1[3] = "1000";
    argv1[4] = NULL;
    createProcess(inc,0, argv1);
  }
  myExit();
}

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_prio.h>

void endless_loop();

uint64_t my_getpid(){
  return getPid();
}

uint64_t my_create_process(char * name){ // devuelve el pid nuevo
  char *args[2];
  args[0] = name;
  args[1] = NULL;
  return createProcess(endless_loop,0,args);;
}

uint64_t my_nice(uint64_t pid, uint64_t newPrio){
  nice(pid,newPrio);
  return newPrio;
}

uint64_t my_kill(uint64_t pid){
  kill(pid);
  return 0;
}

uint64_t my_block(uint64_t pid){
  block(pid);
  return 0;
}

uint64_t my_unblock(uint64_t pid){
  unblock(pid);
  return 0;
}

void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop(){
  uint64_t pid = my_getpid();

  while(1){
    print(STDOUT, "%d ",pid);
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process("endless_loop");

  bussy_wait(WAIT);
  print(STDOUT, "\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 1); //lowest priority 
        break;
      case 1:
        my_nice(pids[i], 5); //medium priority
        break;
      case 2:
        my_nice(pids[i], 10); //highest priority
        break;
    }
  }

  bussy_wait(WAIT);
  print(STDOUT, "\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  print(STDOUT, "CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 3); //medium priority 
        break;
      case 1:
        my_nice(pids[i], 3); //medium priority
        break;
      case 2:
        my_nice(pids[i], 3); //medium priority
        break;
    }
  }

  print(STDOUT, "UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  print(STDOUT, "\nKILLING...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
  myExit();
}

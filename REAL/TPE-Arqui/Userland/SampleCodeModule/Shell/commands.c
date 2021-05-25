#include <commands.h>


void inforeg(char args[MAX_ARGS][MAX_ARG_LEN]){
  clearScreen(0);
  uint64_t registers[19];
  getRegisters(registers);
  putChar(STDOUT, '\n');
  print(STDOUT, "R15: %X - R14: %X\n", registers[0], registers[1]);
  print(STDOUT, "R13: %X - R12: %X\n", registers[2], registers[3]);
  print(STDOUT, "R11: %X - R10: %X\n", registers[4], registers[5]);
  print(STDOUT, "R9: %X - R8: %X\n", registers[6], registers[7]);
  print(STDOUT, "RSI: %X - RDI: %X\n", registers[8], registers[9]);
  print(STDOUT, "RBP: %X - RDX: %X\n", registers[10], registers[11]);
  print(STDOUT, "RCX: %X - RBX: %X\n", registers[12], registers[13]);
  print(STDOUT, "RAX: %X - RIP: %X\n", registers[14], registers[15]);
  print(STDOUT, "CS: %X - FLAGS: %X\n", registers[16], registers[17]);
  print(STDOUT, "RSP: %X\n", registers[18]);
}

void printmem(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  int with0x = 0;
  if (args[1][0] == '0' && args[1][1] == 'x') {
    with0x = 2;
  }
  uint64_t aux = atohex(&args[1][with0x]);
  if (aux >= 0) {
    for (int i = 0; i < 32; i++, aux++) {
      uint64_t mem = 1;
      getMemSyscall(aux, &mem);
      if (mem == 0) {
        print(STDOUT, "-0x%x: 0\n");
      } else {
        print(STDOUT, "-0x%x: %x\n",aux, mem);
      }
    }
  } else {
    print(STDOUT, "INVALID ADDRESS\n");
  }
}

void blockPid(char args[MAX_ARGS][MAX_ARG_LEN]){
  putChar(STDOUT, '\n');
  int pid = atoi(args[1]);
  if (block(pid)==0) 
    print(STDOUT,"Process %s blocked",args[1]);
  else
    print(STDOUT,"Process %s cannot be blocked",args[1]);
}

void unblockPid(char args[MAX_ARGS][MAX_ARG_LEN]){
  putChar(STDOUT, '\n');
  int pid = atoi(args[1]);
  if (unblock(pid)==0) 
    print(STDOUT,"Process %s unblocked",args[1]);
  else
    print(STDOUT,"Process %s cannot be unblocked",args[1]);
}

void ps(char args[MAX_ARGS][MAX_ARG_LEN]) {
  char allProcess[1024*5]; // TODO ajustar el tamño a variable
  // allProcess = malloc(sizeof(char) * 1024);
  psSyscall(allProcess);
  putChar(STDOUT, '\n');
  print(STDOUT, "%s",allProcess);
  // free(allProcess);
}

void test_pipe(char args[MAX_ARGS][MAX_ARG_LEN]){
  print(STDOUT, "\n");
  char *argv[2];
	argv[0] = "writer";
	argv[1] = NULL;
  createProcess(testPipe,0,argv);
}

void getPidS(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  print(STDOUT, "Pid: %d",getPid());
}

void niceS(char args[MAX_ARGS][MAX_ARG_LEN]){
  putChar(STDOUT, '\n');
  int pid = atoi(args[1]);
  int priority = atoi(args[2]);
  if (priority<=0 || pid < 0) {
    print(STDOUT, "Invalid pid/priority");
    return;
  }
  if (nice(pid, priority)>0)
    print(STDOUT, "pid %s priority set to: %s", args[1], args[2]);
  else 
    print(STDOUT, "Cannot set pid %s to priority = %s", args[1], args[2]);
}

void sem(char args[MAX_ARGS][MAX_ARG_LEN]){
  printSem();
}

void chessS(char args[MAX_ARGS][MAX_ARG_LEN]){
  char *argv[2];
	argv[0] = "chess";
	argv[1] = NULL;
  createProcess(chess,1,argv);
}

void killS(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  int pid = atoi(args[1]);
  if (kill(pid)==0)
    print(STDOUT, "Kill pid: %s",args[1]);
  else
    print(STDOUT, "Pid %s cannot be killed",args[1]);
}

void time(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  print(STDOUT, "%d:%d:%d %d/%d/%d", readHours(), readMinutes(), readSeconds(), readDays(), readMonths(), readYear());
}

void divZero(char args[MAX_ARGS][MAX_ARG_LEN]) {
  forceDivZero();
}

void invalidOPCode(char args[MAX_ARGS][MAX_ARG_LEN]) {
  forceInvalidOPCode();
}

void clear(char args[MAX_ARGS][MAX_ARG_LEN]) {
  clearShell();
}

void help(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\nCommands with the (*) symbol can be connected via pipe like this : cmd1 | cmd2\n");
  print(STDOUT, "Commands with the (*) symbol can be ran in background like this : cmd &\n");
  print(STDOUT, "\nAVAILABLE COMMANDS: \n");
  print(STDOUT, "echo [...] - Prints the given arguments in the shell (%d words max)\n", MAX_ARGS);
  print(STDOUT, "chess - Starts a chess game\n");
  print(STDOUT, "printmem [address] - Shows 32 bytes of memory from the given address\n");
  print(STDOUT, "inforeg - Displays the current register snapshot (Press F1 to update)\n");
  print(STDOUT, "clear - clears the shell\n");
  print(STDOUT, "divZero - Forces a divZero exception (For testing purposes)\n");
  print(STDOUT, "invalidOPCode - Forces a invalid opcode exception (For testing purposes)\n");
  print(STDOUT, "time - Displays current time and date\n");
  print(STDOUT, "mem - Prints the Memory State\n");
  print(STDOUT, "ps - Prints all the process pcb\n");
  print(STDOUT, "getPid - Prints the pid of the current process\n");
  print(STDOUT, "kill [Pid] - kills a process given a Pid\n");
  print(STDOUT, "nice - [Pid] [Priority] changes the process priority to the one stated\n");
  print(STDOUT, "blockPid [Pid] - blocks a process given a pid\n");
  print(STDOUT, "unblockPid [Pid] - unblocks a process given a pid\n");
  print(STDOUT, "sem - Prints all the active semaphores\n");
  print(STDOUT, "pipe - Prints all the active pipes\n");
  print(STDOUT, "(*)loop - makes a loop process witch prints its pid\n");
  print(STDOUT, "(*)cat - Prints stdin as it receves it\n");
  print(STDOUT, "(*)wc - Counts the number of lines of the input\n");
  print(STDOUT, "(*)filter - Prints the input without the vowels\n");
  print(STDOUT, "(*)phylo - Phylosophers problem\n");
  print(STDOUT, "(*)filter - Prints the input without the vowels\n");
  print(STDOUT, "test_mm - tests the memory on background, you can stop it with kill\n");
  print(STDOUT, "test_prio - tests the priority changes background\n");
  print(STDOUT, "test_processes - creation of processes on background\n");
  print(STDOUT, "test_sync - tests the process synchronization using semaphores on background\n");
  print(STDOUT, "test_pipe - tests the pipe system implemented in background\n");
  print(STDOUT, "memCheck - prints the state of the memory when not using buddy \n");
}

void echo(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  for (int i = 1; args[i][0] != 0 && i < MAX_ARGS; i++) {
    print(STDOUT, args[i]);
    putChar(STDOUT, ' ');
  }
}

void test_syncS(char args[MAX_ARGS][MAX_ARG_LEN]){
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_sync";
  arg[1] = NULL;
  createProcess(test_sync,0,arg);
}

void test_no_syncS(char args[MAX_ARGS][MAX_ARG_LEN]){
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_no_sync";
  arg[1] = NULL;
  createProcess(test_no_sync,0,arg);
}

void mem(char args[MAX_ARGS][MAX_ARG_LEN]){
  int strSize = 2048; //lo que ntra en una pantalla
  char str[strSize];
  printMemSyscall(str, strSize);
  print(STDOUT, "%s", str);
}

void memCheck(char args[MAX_ARGS][MAX_ARG_LEN]){
  struct checkMemdata data = {0};
  checkMemorySyscall(&data);
  print(STDOUT,"\n");
  print(STDOUT,"* Number of blocks: %d\n", data.numeberOfBlocks);
  print(STDOUT,"    |-> In use: %d\n", data.blockused);
  print(STDOUT,"    |-> Free: %d\n", data.freeBlock);
  print(STDOUT,"\n");
  print(STDOUT,"* Total number of bytes ordered: %d Bytes\n", data.totalBytes);
  print(STDOUT,"    |-> Used in infoBLox: %d Bytes\n", data.bytesUsedByBLocks);
  print(STDOUT,"    |-> Used by the user: %d Bytes\n", data.bytesUsedByUser);
  print(STDOUT,"    |-> Free: %d Bytes\n", data.freeBytes);
  print(STDOUT,"    |-> Lost: %d Bytes\n", data.lostBytes);
  print(STDOUT,"\n");
  print(STDOUT,"* Total errors: %d\n", data.numError);
  print(STDOUT,"    |-> Number of errors A: %d\n", data.freeBlocksTogether);
  print(STDOUT,"    |-> Number of errors B: %d\n", data.curNextPrev);
  print(STDOUT,"    |-> Number of errors C: %d\n", data.memError);
  print(STDOUT,"    |-> Number of errors D: %d\n", data.bytesError);
  /*
  Errores:
    A: dos bloque libreos juntos
    B: que el previous no apunte el bloque anterior (current != current->next->previous))
    C: si memoryDim - firstInfoBlock != data->totalBytes
    D: sumatoria de los bytes no es igual a total bytes
  */

}

void test_memS(char args[MAX_ARGS][MAX_ARG_LEN]){
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_mem";
  arg[1] = NULL;
  createProcess(test_mm,0,arg);
}

void prioTester(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_prio";
  arg[1] = NULL;
  createProcess(test_prio,0,arg);
}

void ProcessTester(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_processes";
  arg[1] = NULL;
  createProcess(test_processes,0,arg);
}

void pipe(char args[MAX_ARGS][MAX_ARG_LEN]){
  printPipe();
}

void catS(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *argv[4];
  argv[0] = "cat";
  argv[1] = args[1];
	argv[2] = args[2];
  argv[3] = NULL;
  createProcess(cat,atoi(args[0]), argv);
  return;
}

void wcS(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *argv[4];
  argv[0] = "wc";
  argv[1] = args[1];
	argv[2] = args[2];
  argv[3] = NULL;
  createProcess(wc,atoi(args[0]), argv);
  return;
}

void filterS(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *argv[4];
  argv[0] = "filter";
  argv[1] = args[1];
	argv[2] = args[2];
  argv[3] = NULL;
  createProcess(filter, atoi(args[0]), argv);
  return;
}

void loopS(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *argv[4];
  argv[0] = "loop";
  argv[1] = args[1];
	argv[2] = args[2];
  argv[3] = NULL;
  createProcess(loop, 0, argv);
}

void phyloS(char args[MAX_ARGS][MAX_ARG_LEN]){
  print(STDOUT, "\n");
  char *argv[4];
	argv[0] = "phyloMaster";
  argv[1] = args[1];
	argv[2] = args[2];
  argv[3] = NULL;
  createProcess(phyloMaster, atoi(args[0]),argv);
}

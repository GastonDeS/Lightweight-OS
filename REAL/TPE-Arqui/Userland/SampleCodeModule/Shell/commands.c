#include <stdio.h>
#include <commands.h>
#include <stdlib.h>
#include <stdGraphics.h>
#include <timer.h>
#include <inforeg.h>
#include <Shell.h>
#include <stdint.h>
#include <syscallsASM.h>
#include <forcedExceptions.h>
#include <timer.h>
#include <test_sync.h>
#include <chess.h>
#include <stddef.h>
#include <checkMemdata.h>
#include <test_mm.h>
#include <test_prio.h>
#include <unistd.h>
#include <test_process.h>
#include <testPipe.h>
#include <phylo.h>
#include <processCom.h>

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
  block(pid);
}

void unblockPid(char args[MAX_ARGS][MAX_ARG_LEN]){
  putChar(STDOUT, '\n');
  int pid = atoi(args[1]);
  unblock(pid);
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
  createProcess(testPipe,1,argv);
}

void getPid(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  uint64_t pid ;
  getPidSyscall(&pid);
  print(STDOUT, "Pid: %d",pid);
}

void niceS(char args[MAX_ARGS][MAX_ARG_LEN]){
  int pid = atoi(args[1]);
  int priority = atoi(args[2]);
  nice(pid, priority);
  print(STDOUT, "pid %s priority set to: %s", args[1], args[2]);
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
  print(STDOUT, "kill pid: %s",args[1]);
  int pid = atoi(args[1]);
  kill(pid);
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
  print(STDOUT, "\nAVAILABLE COMMANDS: \n");
  print(STDOUT, "unblockPid [Pid] - unblocks a process given a pid\n");
  print(STDOUT, "blockPid [Pid] - blocks a process given a pid\n");
  print(STDOUT, "ps - Prints all the process pcb\n");
  print(STDOUT, "getPid - Prints the pid of the current process\n");
  print(STDOUT, "kill [Pid] - kills a process given a Pid\n");
  print(STDOUT, "time - Displays current time and date\n");
  print(STDOUT, "echo [...] - Prints the given arguments in the shell (%d words max)\n", MAX_ARGS);
  print(STDOUT, "chess - Starts a chess game\n");
  print(STDOUT, "printmem [address] - Shows 32 bytes of memory from the given address\n");
  print(STDOUT, "inforeg - Displays the current register snapshot (Press F1 to update)\n");
  print(STDOUT, "clear - clears the shell\n");
  print(STDOUT, "divZero - Forces a divZero exception (For testing purposes)\n");
  print(STDOUT, "invalidOPCode - Forces a invalid opcode exception (For testing purposes)\n");
}

void echo(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar(STDOUT, '\n');
  for (int i = 1; args[i][0] != 0 && i < MAX_ARGS; i++) {
    print(STDOUT, args[i]);
    putChar(STDOUT, ' ');
  }
}

void test_syncS(char args[MAX_ARGS][MAX_ARG_LEN]){
  test_sync();
}

void test_no_syncS(char args[MAX_ARGS][MAX_ARG_LEN]){
  test_no_sync();
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
  print(STDOUT, "\n");
  print(STDOUT, "* Cantidad de bloques: %d\n", data.numeberOfBlocks);
  print(STDOUT, "    |-> Usados: %d\n", data.blockused);
  print(STDOUT, "    |-> Libre: %d\n", data.freeBlock);
  print(STDOUT, "\n");
  print(STDOUT, "* Cantidad total de bytes usados: %d Bytes\n", data.totalBytes);
  print(STDOUT, "    |-> Usados en infoBLocks: %d Bytes\n", data.bytesUsedByBLocks);
  print(STDOUT, "    |-> Usados por el usuario: %d Bytes\n", data.bytesUsedByUser);
  print(STDOUT, "    |-> No utilizados: %d Bytes\n", data.unusedBytes);
  print(STDOUT, "    |-> Usados para alinear: %d Bytes\n", data.bytesUsedByAlign);
  print(STDOUT, "    |-> Bytes perdidos: %d Bytes\n", data.lostBytes);
  print(STDOUT, "\n");
  print(STDOUT, "* Numero de errores: %d\n", data.numError);
  print(STDOUT, "    |-> Numero de bloque con error A: %d\n", data.freeBlocksTogether);
  print(STDOUT, "    |-> Numero de bloque con error B: %d\n", data.noAlignBlocks);
  print(STDOUT, "    |-> Numero de bloque con error C: %d\n", data.curNextPrev);
  /*
  Errores:
    A: dos bloque libreos juntos
    B: bloque no alineado a 8 bytes
    C: que el previous no apunte el bloque anterior
  */

}

void test_memS(char args[MAX_ARGS][MAX_ARG_LEN]){
  test_mm();
}

void prioTester(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_prio";
  arg[1] = NULL;
  createProcess(test_prio,1,arg);
}

void ProcessTester(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print(STDOUT, "\n");
  char *arg[2];
  arg[0] = "test_processes";
  arg[1] = NULL;
  createProcess(test_processes,1,arg);
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

//odas las que terminan con S (ejempo wcS) no son built in
//reciben en args[0] si son foreground o background,
//en args[1] el pipePid de entrada
//en args[2] el pipePid de salida

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

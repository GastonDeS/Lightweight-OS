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

void inforeg(char args[MAX_ARGS][MAX_ARG_LEN]){
  clearScreen(0);
  uint64_t registers[19];
  getRegisters(registers);
  putChar('\n');
  print("R15: %X - R14: %X\n", registers[0], registers[1]);
  print("R13: %X - R12: %X\n", registers[2], registers[3]);
  print("R11: %X - R10: %X\n", registers[4], registers[5]);
  print("R9: %X - R8: %X\n", registers[6], registers[7]);
  print("RSI: %X - RDI: %X\n", registers[8], registers[9]);
  print("RBP: %X - RDX: %X\n", registers[10], registers[11]);
  print("RCX: %X - RBX: %X\n", registers[12], registers[13]);
  print("RAX: %X - RIP: %X\n", registers[14], registers[15]);
  print("CS: %X - FLAGS: %X\n", registers[16], registers[17]);
  print("RSP: %X\n", registers[18]);
}

void printmem(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar('\n');
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
        print("-0x%x: 0\n");
      } else {
        print("-0x%x: %x\n",aux, mem);
      }
    }
  } else {
    print("INVALID ADDRESS\n");
  }
}

void time(char args[MAX_ARGS][MAX_ARG_LEN]) { 
  putChar('\n');
  print("%d:%d:%d %d/%d/%d", readHours(), readMinutes(), readSeconds(), readDays(), readMonths(), readYear());
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
  print("\nAVAILABLE COMMANDS: \n");
  print("time - Displays current time and date\n");
  print("echo [...] - Prints the given arguments in the shell (%d words max)\n", MAX_ARGS);
  print("chess - Starts a chess game\n");
  print("printmem [address] - Shows 32 bytes of memory from the given address\n");
  print("inforeg - Displays the current register snapshot (Press F1 to update)\n");
  print("clear - clears the shell\n");
  print("divZero - Forces a divZero exception (For testing purposes)\n");
  print("invalidOPCode - Forces a invalid opcode exception (For testing purposes)\n");
  print("UWU - Surprise");
}

void echo(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar('\n');
  for (int i = 1; args[i][0] != 0 && i < MAX_ARGS; i++) {
    print(args[i]);
    putChar(' ');
  }
}

void uwu(char args[MAX_ARGS][MAX_ARG_LEN]) {
  print("\n\n\n");
  print("  @@@@            @@@@                                    (@@@&           *@@@& \n");
  print("  @@@@            @@@@                                    (@@@&           *@@@& \n");
  print("  @@@@            @@@@    @@@@         &@&        @@@@    (@@@&           *@@@& \n");
  print("  @@@@            @@@@    ,@@@@       @@@@@       @@@@    (@@@&           *@@@& \n");
  print("  @@@@            @@@@     &@@@&     .@@@@@@     .@@@&    (@@@&           *@@@& \n");
  print("  @@@@            @@@@      @@@@     @@@ @@@(    @@@@     (@@@&           *@@@& \n");
  print("  @@@@.           @@@@       @@@@   @@@@ /@@@    @@@*     ,@@@@           #@@@# \n");
  print("   @@@@.         @@@@#       ,@@@. &@@@   @@@@  @@@#       @@@@@         ,@@@@  \n");
  print("    @@@@@@@&#&@@@@@@          @@@@@@@@     @@@@@@@,         *@@@@@@&#&@@@@@@&   \n");
  print("       &@@@@@@@@@.             (@@@@        @@@@#              /@@@@@@@@@&      \n");
  print("\n\n\n");
}

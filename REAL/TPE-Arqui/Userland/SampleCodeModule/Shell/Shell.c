#ifndef SHELL
#define SHELL

#define TOTAL_LINES 63
#define MAX_LINE_LENGTH 129

#include <stdGraphics.h>
#include <stdio.h>
#include <stdint.h>
#include <colors.h>
#include <commands.h>
#include <Shell.h>
#include <stdlib.h>
#include <timer.h>
#include <unistd.h>
#include <pipe.h>

static char lines[TOTAL_LINES][MAX_LINE_LENGTH];// = {0};
static char buffShell[MAX_LINE_LENGTH];
static int shellCursor = 0;
static uint8_t lineCursor = 0;
static uint8_t currentLineNumber = 0;

void exceptions();
static void addLine();
static void drawShellLines();
static void clearShellLine(uint8_t line);
static void drawBottomLine();
static void clearScreenLine(uint8_t line);
static void exeCommand(char*);
static int isCommand(char * name);
void updateShell(char * buff, int dim);


static int iSbuiltIn(char *name);
static int isPipe(char *name);


char noBuiltIn[][MAX_ARG_LEN]={"loop","filter","wc","cat","phylo"};
char commandsNames[][MAX_ARG_LEN]={"pipe","loop","filter","wc","cat","phylo","test_pipe","test_no_sync","test_mem","memCheck","mem","sem","test_processes","test_prio","test_sync","nice","unblockPid","blockPid","ps","getPid","kill","time","help","inforeg","chess","printmem","divZero","invalidOPCode","clear","echo"};
void  (* run[])(char args[MAX_ARGS][MAX_ARG_LEN]) = {pipe,loopS,filterS,wcS,catS,phyloS,test_pipe,test_no_syncS,test_memS,memCheck,mem,sem,ProcessTester,prioTester,test_syncS,niceS,unblockPid,blockPid,ps,getPid,killS,time,help,inforeg,chessS,printmem,divZero,invalidOPCode,clear,echo};
static int totalCommands = 30;
static int totalNoBuiltIn = 5;

void init_shell(int argc, char **argv) {
  setConsoleUpdateFunction(updateShell);
  drawShellLines();
  exceptions();
  while (1) {
		int ch = getChar();
		keyPressedShell(ch);
	}
}
void writeToLines(char * buff, int dim) {
  for (int i = 0; i < dim && buff[i] != 0 /*&& i < MAX_LINE_LENGTH*/; i++) {
    if (buff[i] == '\n' || lineCursor == (MAX_LINE_LENGTH - 1)) { //El -1 es para que el ultimo elemento sea un 0
      addLine();

    } else if (buff[i] == '\b') {
      if (lineCursor > 0) {
        lines[currentLineNumber%(TOTAL_LINES-1)][lineCursor-1] = lines[currentLineNumber%(TOTAL_LINES-1)][lineCursor];
        lineCursor-=lineCursor==0?0:1; //TODO: checkear por que el lineCursor-- se va negativo aunque no deberia entrar al if
      }
    } else {
      lines[currentLineNumber%(TOTAL_LINES-1)][lineCursor] = buff[i];
      lineCursor++;
    }
  }
  drawBottomLine();
}

static void addLine() {
  currentLineNumber++;
  clearShellLine(currentLineNumber);
  lineCursor=0;
  drawShellLines();
}

static void drawShellLines() {
  clearScreen(BLACK);
  int y = SCREEN_HEIGHT;
  int x = 0;
  for (int i = 0; i >= -TOTAL_LINES && i >= -currentLineNumber; i--) {
    y-=BASE_CHAR_HEIGHT;
    if (lines[(i+currentLineNumber)%(TOTAL_LINES-1)][0] == 0) continue;
    if (i == 0) {
      drawString(0, y, "> ", 3, LIGHT_GRAY, BLACK, 1, 0);
      x += BASE_CHAR_WIDTH*2;
    } else {
      x = 0;
    }
    drawString(x, y, lines[(i+currentLineNumber)%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, LIGHT_GRAY, BLACK, 1, 0);
  }
}

static void clearShellLine(uint8_t line) {
  for (int i = 0; i < MAX_LINE_LENGTH; i++) {
    lines[line%(TOTAL_LINES-1)][i] = 0;
  }
}

static void drawBottomLine() {
  clearScreenLine(0);
  drawString(0, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, "> ", 3, LIGHT_GRAY, BLACK, 1, 0);
  drawString(BASE_CHAR_WIDTH*2, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, lines[(currentLineNumber)%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, LIGHT_GRAY, BLACK, 1, 0);
}

static void clearScreenLine(uint8_t line){
  drawRect(0,SCREEN_HEIGHT-BASE_CHAR_HEIGHT*(line+1),SCREEN_WIDTH,BASE_CHAR_HEIGHT, BLACK);
}

//ejecutaria los commands
static void exeCommand(char * line){
  char argv[10][32] = {{0}};
  char commandArgs[10][32] = {{0}}; //Maximo 10 argumentos de 32 caracteres c/u
  int foundArgs = 0;
  int index = 0;
  int nameIndex = 0;
  while (line[index] != 0 && line[index] != '\n' && foundArgs < 10) {
    if (line[index] != ' ' && line[index] != '-') {
      commandArgs[foundArgs][nameIndex++] = line[index];
    }
    else if (line[index] == ' ') {
      foundArgs++;
      nameIndex = 0;
    }
    index++;
  }
  for (; shellCursor >= 0 ; shellCursor--) {
    buffShell[shellCursor] = 0;
  }

  int i = isCommand(commandArgs[0]);
  if(i == -1){
    print(" - INVALID COMMAND");
    return;
  }

  if(!isPipe(commandArgs[1])){
    if( iSbuiltIn(commandArgs[0])){
      intToString(-1, argv[0]);
      intToString(1, argv[1]);
      run[i](argv);
      return;
    }

    run[i](commandArgs);
    return;
  }

  if( iSbuiltIn(commandArgs[0]) || iSbuiltIn(commandArgs[2])){
    print(" - INVALID PIPE ");
    return;
  }

  int pipeId = pipeCreate();
  if(pipeId == -1){
    print(" - PIPE ERROR ");
    return;
  }

  //i = isCommand(commandArgs[0]);
  intToString(pipeId, argv[0]);
  intToString(1, argv[1]);
  run[i](argv);

  i = isCommand(commandArgs[2]);
  intToString(0, argv[1]);
  run[i](argv);
}


static int iSbuiltIn(char *name){
    for (int i = 0; i < totalNoBuiltIn; i++) {
      if( strcmp(noBuiltIn[i], name) == 0)
          return 0;
    }
    return 1;
}

static int isPipe(char *name){
  return name[0] == '|';
}

//devuelve que comando es si no esta  devuelve -1
static int isCommand(char * name){
  for (int i = 0; i < totalCommands; i++) {
    if (!strcmp(commandsNames[i],name)){
      return i;
    }
  }
  return -1;
}

void keyPressedShell(char ch) {
  if (ch) {
    if (ch == '\n' && lineCursor > 0) {
      if (lineCursor!=shellCursor)
        print("\n%s",buffShell);
      exeCommand(buffShell);
       //lines[(currentLineNumber)%(TOTAL_LINES-1)
    }
    buffShell[shellCursor++] = ch;
    putChar(ch);
  }
}

void updateShell(char * buff, int dim) {
  writeToLines(buff, dim);
}

void clearShell() {
  currentLineNumber = 0;
  clearShellLine(0);
}

void exceptions(){
  uint64_t error = getError();
    if (error < 32) {
      uint64_t registers[19];
      getRegistersSyscall(registers);
      print("ERROR: EXCEPTION %x ", error);
      switch (error) {
        case 0: print("(DIVISION BY ZERO)\n");
        break;
        case 1: print("(INVALID OPCODE)\n");
      }
      print("REGISTERS STATUS:\n");
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
    } else {
      print("What module would you like to run? (type \"help\" to see commands)\n");
    }
}

#endif

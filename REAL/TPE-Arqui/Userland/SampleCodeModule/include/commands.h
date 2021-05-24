#ifndef COMMANDS_H
#define COMMANDS_H
#define MAX_ARGS 10
#define MAX_ARG_LEN 32

#include <stdio.h>
#include <stdlib.h>
#include <stdGraphics.h>
#include <timer.h>
#include <inforeg.h>
#include <Shell.h>
#include <stdint.h>
#include <syscallsASM.h>
#include <forcedExceptions.h>
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


void phyloS(char args[MAX_ARGS][MAX_ARG_LEN]);
void test_pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
void test_memS(char args[MAX_ARGS][MAX_ARG_LEN]);
void ProcessTester(char args[MAX_ARGS][MAX_ARG_LEN]);
void memCheck(char args[MAX_ARGS][MAX_ARG_LEN]);
void mem(char args[MAX_ARGS][MAX_ARG_LEN]);
void sem(char args[MAX_ARGS][MAX_ARG_LEN]);
void test_syncS(char args[MAX_ARGS][MAX_ARG_LEN]);
void test_no_syncS(char args[MAX_ARGS][MAX_ARG_LEN]);
void prioTester(char args[MAX_ARGS][MAX_ARG_LEN]);
void niceS(char args[MAX_ARGS][MAX_ARG_LEN]);
void chessS(char args[MAX_ARGS][MAX_ARG_LEN]);
void blockPid(char args[MAX_ARGS][MAX_ARG_LEN]);
void unblockPid(char args[MAX_ARGS][MAX_ARG_LEN]);
void ps(char args[MAX_ARGS][MAX_ARG_LEN]);
void getPid(char args[MAX_ARGS][MAX_ARG_LEN]);
void killS(char args[MAX_ARGS][MAX_ARG_LEN]);
void help(char args[MAX_ARGS][MAX_ARG_LEN]);
void echo(char args[MAX_ARGS][MAX_ARG_LEN]);
void inforeg(char args[MAX_ARGS][MAX_ARG_LEN]);
void time(char args[MAX_ARGS][MAX_ARG_LEN]);
void printmem(char args[MAX_ARGS][MAX_ARG_LEN]);
void divZero(char args[MAX_ARGS][MAX_ARG_LEN]);
void invalidOPCode(char args[MAX_ARGS][MAX_ARG_LEN]);
void clear(char args[MAX_ARGS][MAX_ARG_LEN]);
void pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
void catS(char args[MAX_ARGS][MAX_ARG_LEN]);
void wcS(char args[MAX_ARGS][MAX_ARG_LEN]);
void filterS(char args[MAX_ARGS][MAX_ARG_LEN]);
void loopS(char args[MAX_ARGS][MAX_ARG_LEN]);
#endif

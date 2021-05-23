#ifndef COMMANDS_H
#define COMMANDS_H
#define MAX_ARGS 10
#define MAX_ARG_LEN 32

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
#endif

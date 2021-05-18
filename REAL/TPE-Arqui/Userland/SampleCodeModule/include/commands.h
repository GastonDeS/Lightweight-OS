#ifndef COMMANDS_H
#define COMMANDS_H
#define MAX_ARGS 10
#define MAX_ARG_LEN 32
void semTester(char args[MAX_ARGS][MAX_ARG_LEN]);
void test(char args[MAX_ARGS][MAX_ARG_LEN]);
void nice(char args[MAX_ARGS][MAX_ARG_LEN]);
void blockPid(char args[MAX_ARGS][MAX_ARG_LEN]);
void unblockPid(char args[MAX_ARGS][MAX_ARG_LEN]);
void listAllProcess(char args[MAX_ARGS][MAX_ARG_LEN]);
void getPid(char args[MAX_ARGS][MAX_ARG_LEN]);
void kill(char args[MAX_ARGS][MAX_ARG_LEN]);
void help(char args[MAX_ARGS][MAX_ARG_LEN]);
void echo(char args[MAX_ARGS][MAX_ARG_LEN]);
void inforeg(char args[MAX_ARGS][MAX_ARG_LEN]);
void time(char args[MAX_ARGS][MAX_ARG_LEN]);
void printmem(char args[MAX_ARGS][MAX_ARG_LEN]);
void divZero(char args[MAX_ARGS][MAX_ARG_LEN]);
void invalidOPCode(char args[MAX_ARGS][MAX_ARG_LEN]);
void clear(char args[MAX_ARGS][MAX_ARG_LEN]);
void uwu(char args[MAX_ARGS][MAX_ARG_LEN]);
#endif

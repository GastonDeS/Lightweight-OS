#include <stdint.h>
#include <commands.h>
#include <unistd.h>
#include <syscallsASM.h>
#include <stdio.h>
#include <stddef.h>

#define MINOR_WAIT 10000000                               // TODO: To prevent a process from flooding the screen
#define WAIT       100000000                              // TODO: Long enough to see theese processes beeing run at least twice

void test_prio();

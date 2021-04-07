#ifndef  SHELL_H
#define SHELL_H
#include <stdint.h>

void init_shell();
void writeToLines(char * buff, int dim);
void keyPressedShell(char);
void clearShell();
#endif

#ifndef PHYLO
#define PHYLO

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <mySemaphore.h>
#include <syscallsASM.h>

#define INITIALPHYLOFERS 5
#define MAXPHYLOS 10
#define SEMPHYLO "phylos"

void phyloMaster(int argc, char **argv);

#endif
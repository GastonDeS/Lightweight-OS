#ifndef PROCESSCOM
#define PROCESSCOM

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stddef.h>
#include <processCom.h>



void cat(int argc, char **argv);
void wc(int argc, char **argv);
void filter(int argc, char **argv);
void loop(int argc, char **argv);
#endif
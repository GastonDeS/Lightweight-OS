#include <test_util.h>
#include <stdio.h>
#include <stdGraphics.h>
#include <stdlib.h>
#include <stddef.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 10000    //Should be around 80% of memory managed by the MM
#define N 10                //cantidad de pasadas

void test_mm();
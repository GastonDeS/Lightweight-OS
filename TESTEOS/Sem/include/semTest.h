#ifndef SEM_TEST
#define SEM_TEST

#include "CuTest.h"

typedef void (*Test)(CuTest *const cuTest);

CuSuite *getSemTestSuite(void);

#endif
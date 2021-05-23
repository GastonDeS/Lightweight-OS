#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

int atoi(char * str);
int intToString(int num, char * buffer);
int strlen(char *str);
int strcmp(char * str1, char * str2);
int intToBase(unsigned long long num, int base, char*buffer);
int iabs(int num);
char * strcpy(char * dest, char * src);
int atohex(char * str);
char toUpper(char letter);
char toLower(char letter);
int strcat2(char * dest, int* from, int strSize, char * src);
void addSpace(char *dest, int* from, int strSize, int numSpace);
//crea un copia de str. Usa malloc para crear el nuevo string, hay que 
//liberarlo con free luego de usarlo
char* strCopy(char* str);
#endif

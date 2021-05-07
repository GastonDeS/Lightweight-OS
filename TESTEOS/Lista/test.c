#include "listADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>



//aux function:
int equalsInt(void* n1, void* n2);
int equalsChar(void* n1, void* n2);
int equalsDouble(double* n1, double* n2);
int equalStrings(void* n1, void* n2);
void printInt(void* n1);
void printChar(void *c);
void printDouble(void *c);
void printStrings(void *c);

void main(){
    srand(time(0));

    char auxChar[26];
    int auxInt[40];

    listADT list;


    /*
    printf("\n---------CHARACTERES------------\n");

    list = newList(sizeof(char),equalsChar);

    for(int i=0;i<26;i++){
        auxChar[i] = i+'A';
        insert(list,&auxChar[i]);
    }

    printList(list,printChar);

    printf("\n pop1 %c", *((char*)pop(list)) );
    printf("\n pop2 %c", *((char*)pop(list)) );
    printf("\n pop3 %c \n\n", *((char*)pop(list)) );

    printList(list,printChar);

    printf("cambio la R por r \n\n");
    char aux = 'R';
    char* value = (char*)getElem(list, &aux);
    *value = 'r';
    printList(list,printChar);

    printf("cambio la E por S \n\n");
    aux = 'E';
    value = (char*)getElem(list, &aux);
    *value = 'S';
    printList(list,printChar);

    free(list);
    printList(list,printChar);
    */

   /*
    printf("\n---------interger------------\n");

    list = newList(sizeof(int),equalsInt);

    
    for(int i=0;i<40;i++){
        auxInt[i] = i;
        insert(list,&auxInt[i]);
    }
    printList(list,printInt);
    printf("\n---------------------------\n");
    
    for (int j = 0; j < 10; j++){
        for (int i = 0; i < 3; i++){
            int aux  = *( (int*)next(list) );
            //printf("\n %d \n", aux);
        }
        int aux = 0;
        insertBeforeNext(list, &aux);
    }
    printList(list,printInt);
    printf("\n---------------------------\n");
    
    cleanList(list);
    printList(list,printInt); 
    printf("\n---------------------------\n");

    for(int i=0;i<40;i++){
        auxInt[i] = i;
        addToTheEnd(list,&auxInt[i]);
    }
    printList(list,printInt);
    printf("\n---------------------------\n");
    */

   printf("\n---------delete------------\n");

   list = newList(sizeof(char),equalsChar);
    char c1 = 'A';
    char c2 = 'X';
    for(int i=0;i<20;i++){
        if(i%5)
            insert(list,&c1);
        else
            insert(list,&c2);
    }
    printList(list,printChar);
    printf("\n---------------------------\n");

    for (int j = 0; j < 4; j++){
        for(int i = 0; i < 5; i++)
            next(list);
        deleteCurrentElem(list);
    }
    printList(list,printChar);
    printf("\n---------------------------\n");

    elementBelongs(list, &c1);

    printf("queda una x?: %s \n\n", elementBelongs(list, &c1)? "si" : "no");

    delete(list, &c2);
    printList(list,printChar);
}



int equalsChar(void* n1, void* n2){
    return *((char*)n1) == *((char*)n2);
}

int equalsStrings(void* n1, void* n2){
    return strcmp(*((char**)n1),*((char**)n2)) == 0;
}

int equalsDouble(double* n1, double* n2){
    return *n1 == *n2;
}

int equalsInt(void* n1, void* n2){
    return *((int*)n1) == *((int*)n2);
}

void printDouble(void *c){
    printf("**%f**\n",*((double*)c));
}
void printStrings(void *c){
    printf("**%s**\n",*((char**)c));
}

void printChar(void *c){
    printf("**%c**\n",*((char*)c));
}
void printInt(void* n1){
    printf("||%d||\n",*((int*)n1));
}
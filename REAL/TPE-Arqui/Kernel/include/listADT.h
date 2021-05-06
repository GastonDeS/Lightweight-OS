#ifndef LISTADT_H_
#define LISTADT_H_

typedef struct listCDT *listADT;


listADT newList(int elemSize, int (*equals)(void* elem1, void* elem2));


int insertBeforeNext(listADT list, void* element);

int addToTheEnd(listADT list, void* element);

//inserta los elementos al principio de la lista
int insert(listADT  list, void* element);


void* pop(listADT list);


int deleteCurrentElem(listADT list);

int delete(listADT list, void* element);


int isEmpty(const listADT list);

int size(const listADT list);

void freeList(listADT list);


int elementBelongs(const listADT list, void* element);

void* getElem(const listADT list, void* element);




/**
 * Funciones para poder iterar sobre la lista
 */
void toBegin(listADT list);

int hasNext(listADT list);

void* next(listADT list);

#endif 

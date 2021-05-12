#ifndef LISTADT_H_
#define LISTADT_H_

typedef struct listCDT *listADT;


/*return: -1 si hubo un error
**         1 si lo encotro (en la funciones que coresponda)
**         0 no lo encontro / funciono bien
*/


//crea una nueva lista
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

//borra todos los elemntos de la lista, se puede seguir usando
void cleanList(listADT list);

//borra la lista en si, no se puede seguir usando
void freeList(listADT list);


int elementBelongs(const listADT list, void* element); 

void* getElem(const listADT list, void* element);


/**
 * Funciones para poder iterar sobre la lista
 */
void toBegin(listADT list);

int hasNext(listADT list);

void* next(listADT list);


//debugger
void printList(listADT list,void (*print)(void* n));

#endif 

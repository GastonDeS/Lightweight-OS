#include <listADT.h>

typedef struct node {
	void* value;
	struct node * next;
    struct node * previous;
}node;
typedef struct node * nodeP;

typedef struct listCDT{
	nodeP first;
    nodeP iteradorNext; 
	int valueBytes;
    int (*equals)(void* elem1, void* elem2);
    unsigned int size;
}listCDT;

//prtivate:
int search(nodeP* current, void* element, int (*comparator)(void*, void*));
nodeP createNode(int valueBytes, void* element);
void removeNode(nodeP current, listADT list);


listADT newList(int elemSize, int (*equals)(void* elem1, void* elem2)){
    listADT list = malloc(sizeof(struct listCDT));
    list->first = NULL;
    list->valueBytes = elemSize;
    list->equals = equals;
    list->size = 0;
    list->iteradorNext = NULL;
    return list;
}

int insertBeforeNext(listADT list, void* element){
    if(!hasNext(list)) // no se inicializo iteradorNext
        toBegin(list);

    if(list->iteradorNext == list->first)
        return addToTheEnd(list, element);

    nodeP aux = createNode(list->valueBytes, element);
    if(aux == NULL)
        return -1;

    list->iteradorNext->previous->next = aux;
    aux->previous = list->iteradorNext->previous;
    aux->next = list->iteradorNext;
    list->iteradorNext->previous = aux; 
    list->size++;

    return 0;
}

int addToTheEnd(listADT list, void* element){
    
    if(isEmpty(list))//es el primer elemento
        return insert(list, element);

    nodeP aux = createNode(list->valueBytes, element);
    if(aux == NULL)
        return -1;
        
    aux->next = NULL;
    nodeP current = list->first;
    while(current->next != NULL )
        current = current->next;
    current->next = aux;
    aux->previous = current;
    list->size++;
    return 0;
}

//inserta los elementos al principio de la lista
int insert(listADT list, void* element){
    
    nodeP aux = createNode(list->valueBytes, element);
    if(aux == NULL)
        return -1;
    
    aux->previous = NULL;
    aux->next = list->first;
    list->first = aux;
    if(aux->next != NULL)
        aux->next->previous = aux;
    list->size++;
    return 0;
}

//si hubo un error devulve NULL
void* pop(listADT list){
    if(isEmpty(list))
        return NULL;
    
    void* result = malloc(list->valueBytes);
    memcpy(result, list->first->value, list->valueBytes);

    deleteFirstElem(list);
    return result;
}

int deleteFirstElem(listADT list){
    if(isEmpty(list))
        return -1;

    removeNode(list->first, list);

    return 0;
}

int deleteCurrentElem(listADT list){
    if(!hasNext(list))//si no lo fue inicializado, se inicializa 
        toBegin(list);

    //variable auxiliares
    nodeP deleteNode = list->iteradorNext->previous;
    if(deleteNode == NULL){ //el elemento actual esta al final de la lista
        deleteNode = list->first;
        while(deleteNode->next == NULL)
            deleteNode = deleteNode->next;
        removeNode(deleteNode, list);
        return 0;
    }

    nodeP previousNode = deleteNode->previous; //guardo el nodo anteriror
    removeNode(deleteNode, list); //elimino el node

    list->iteradorNext->previous = previousNode;
    if(previousNode != NULL)
        previousNode->next = list->iteradorNext;
    else
        list->first = list->iteradorNext;
    
    return 0;
}

//se le podria pasar un puntero a funcion del compare
//retorna 1 si lo elimino y 0 si no lo encontro 
int delete(listADT list, void* element){
    nodeP current = list->first;
    if(!search(&current, element, list->equals))
        return 0;

    removeNode(current, list);

    return 1;
}

int deleteElem(listADT list, void* element, void (*deleteElemValue)(void* value)){
    
    nodeP current = list->first;
    if(!search(&current, element, list->equals))
        return 0;
    
    if(current->next != NULL)
        current->next->previous = current->previous;
    if(current->previous != NULL)
        current->previous->next = current->next;
    else
        list->first = current->next;
    list->size --;

    deleteElemValue(current->value);
    free(current);

    return 1;
}

void removeNode(nodeP current, listADT list){

    if(current == NULL)
        return;

    if(current->next != NULL)
        current->next->previous = current->previous;
    if(current->previous != NULL)
        current->previous->next = current->next;
    else
        list->first = current->next;
    list->size --;
    free(current->value);
    free(current);
}

int isEmpty(const listADT list){
	return size(list) == 0;
}

int size(const listADT list) {
	return list->size;
}

int elementBelongs(const listADT list, void* element){
	return search(&(list->first), element, list->equals);
}

void* getElem(const listADT list, void* element){
    nodeP elem = list->first;
    if(search(&elem, element, list->equals))
        return elem->value;
    return NULL;
}

void* getCurrentElem(const listADT list){
    return list->iteradorNext->value;
}

void cleanList(listADT list){
    nodeP current = list->first, aux;
	while (current != NULL) {
		aux = current->next;
		free(current->value);
        free(current);
		current = aux;
	}
    list->first = NULL;
    list->size = 0;
    list->iteradorNext = NULL;
}

void freeList( listADT list){
	cleanList(list);
    free(list);
}

//   ITERADOR
void toBegin(listADT list) {
	list->iteradorNext = list->first;
}

int hasNext(const listADT list) {
	return list->iteradorNext != NULL;
}

void* next(listADT list) {
	if(!hasNext(list))
		toBegin(list);

    void* result = list->iteradorNext->value;
	
    list->iteradorNext = list->iteradorNext->next;

	return result;
}

//private:
//si retorna 1 lo encontro sino -1 y en current esta el nodo buscado
int search(nodeP* current, void* element, int (*equals)(void*, void*)) {
    while (*current != NULL){
        if(equals((*current)->value, element))
            return 1;
        *current = (*current)->next;
    }
    return 0;     
}

nodeP createNode(int valueBytes, void* element){
    nodeP aux = malloc(sizeof(struct node));
    if(aux == NULL)
        return NULL;
    aux->next = NULL;
    aux->value = malloc(valueBytes);
    if(aux->value == NULL)
        return NULL;
    memcpy(aux->value, element, valueBytes);
    return aux;
}
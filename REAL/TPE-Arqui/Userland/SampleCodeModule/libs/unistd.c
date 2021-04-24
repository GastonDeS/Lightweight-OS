#include <unistd.h>

void createProcess(void (*function),char **argv){
    //TODO tratar el vector que viene capaz y 
    //cambiarlo de char ** a un char * para que al 
    //recibir los * uno desp pase los argumentos como quiera
    createProcessSyscall(function,argv);
}
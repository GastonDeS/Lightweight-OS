#include <phylo.h>

typedef enum {WAITING,EATING}State; 

typedef struct {
    uint64_t pid;
    State state;
}Phylosofer;


int forks[MAXPHYLOS];
Phylosofer phylosofers[MAXPHYLOS];

void addFork(int number);
void launchPhylosofer(int number);
void admin(int semId);

void phylosofer(int argc, char **argv){
    int semId = sem_open(SEMPHYLO,1);
    int phyloN = atoi(argv[1]);
    char forkLS[5];
    int aux = phyloN;//(phyloN-1+INITIALPHYLOFERS)%INITIALPHYLOFERS;
    intToString(aux,forkLS);
    int forkL = sem_open(forkLS,1);
    char forkRS[5];
    aux = (phyloN+1)%INITIALPHYLOFERS;
    intToString(aux,forkRS);
    int forkR = sem_open(forkRS,1);

    while (1) {
        if (phyloN%2) {
            if (phylosofers[phyloN].state) {
                sem_wait(semId);
                phylosofers[phyloN].state = WAITING;
                sem_post(semId);
                sem_post(forkR);
                sem_post(forkL);
            } else {
                sem_wait(forkL);
                sem_wait(forkR);
                sem_wait(semId);
                phylosofers[phyloN].state = EATING;
                sem_post(semId);
            }
        } else {
            if (phylosofers[phyloN].state) {
                sem_wait(semId);
                phylosofers[phyloN].state = WAITING;
                sem_post(semId);
                sem_post(forkL);
                sem_post(forkR);
            } else {
                sem_wait(forkR);
                sem_wait(forkL);
                sem_wait(semId);
                phylosofers[phyloN].state = EATING;
                sem_post(semId);
            }
        }
        yieldSyscall();
    }
    
}

void phyloMaster(int argc, char **argv) {

    int semId = sem_open(SEMPHYLO,1);
    sem_wait(semId);
    for (int i = 0; i < INITIALPHYLOFERS; i++) {
        launchPhylosofer(i);
        addFork(i);
    }
    sem_post(semId);
    
    admin(semId);

    myExit();
}

void admin(int semId) {
    while (1) {
        sem_wait(semId);
        for (int i = 0; i < INITIALPHYLOFERS; i++) {
            char aux[5];
            intToString(phylosofers[i].state,aux);
            print(aux);
        }
        print("\n");
        sem_post(semId);
        yieldSyscall();
    }
}

void addFork(int number) {
    char *name = malloc(sizeof(char)*5); 
    intToString(number,name);
    forks[number] = sem_open(name,1);
}

void launchPhylosofer(int number) {
    char *args[3];
    args[0] = "phylosofer";
    char aux[10];
    intToString(number,aux); 
    args[1] = aux;
    args[2] = NULL;
    phylosofers[number].pid = createProcess(phylosofer,args);
    phylosofers[number].state = WAITING;
}
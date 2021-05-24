#include <phylo.h>

typedef enum {WAITING,EATING}State; 
typedef enum {ALIVE,DEAD,REFRESHADD,REFRESHREM}IsAlive;

typedef struct {
    uint64_t pid;
    State state;
    IsAlive isAlive;
}Phylosofer;

int phylosofersCount = INITIALPHYLOSOFERS;
int forks[MAXPHYLOS];
Phylosofer phylosofers[MAXPHYLOS];

void addFork(int number);
void launchPhylosofer(int number);
void admin(int semId);
void removePhylosofer();
void addPhylosopher();
void closePhylosfers(int semId);
void refreshAdd(int *forkR);
void refreshRem(int *forkR);

void phylosofer(int argc, char **argv){
    int semId = sem_open(SEMPHYLO,1);
    int phyloN = atoi(argv[1]);
    char forkLS[5];
    int aux = phyloN;
    intToString(aux,forkLS);
    int forkL = sem_open(forkLS,1);
    char forkRS[5];
    aux = (phyloN+1)%phylosofersCount;
    intToString(aux,forkRS);
    int forkR = sem_open(forkRS,1);

    while (phylosofers[phyloN].isAlive!=DEAD) {
        if (phylosofers[phyloN].isAlive==REFRESHREM && phylosofers[phyloN].state!=EATING) 
            refreshRem(&forkR);
        else if(phylosofers[phyloN].isAlive==REFRESHADD && phylosofers[phyloN].state!=EATING) {
            refreshAdd(&forkR);
        }
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
    if (phylosofers[phyloN].state==EATING) {
        if (phyloN%2){
            sem_post(forkL);
            sem_post(forkR);
        } else {
            sem_post(forkL);
            sem_post(forkR);
        }
    }
    sem_close(forkL);
    sem_close(forkR);
    sem_close(semId);
    myExit();
}

void phyloMaster(int argc, char **argv) {
    phylosofersCount = INITIALPHYLOSOFERS;
    int semId = sem_open(SEMPHYLO,1);
    sem_wait(semId);
    for (int i = 0; i < phylosofersCount; i++) {
        launchPhylosofer(i);
        addFork(i);
    }
    sem_post(semId);
    
    admin(semId);

    unblock(0);
    myExit();
}

void admin(int semId) {
    while (1) {
        char buf[1];
        buf[0] = 0;
        if (readKeyboard(buf, 1)) {
            if (buf[0]=='e'){
                closePhylosfers(semId);
                return;
            } else if (buf[0] == 'a' && phylosofersCount<MAXPHYLOS ){
                addPhylosopher();
            } else if (buf[0] == 'r' && phylosofersCount>MINPHYLOS) {
                removePhylosofer();
            }
        }
        sem_wait(semId);
        for (int i = 0; i < phylosofersCount; i++) {
            char aux[5];
            intToString(phylosofers[i].state,aux);
            print(aux);
        }
        print("\n");
        sem_post(semId);
        yieldSyscall();
    }
}

void refreshRem(int *forkR){
    sem_close(*forkR);
    (*forkR) = sem_open("0",1);;
}

void refreshAdd(int *forkR){
    sem_close(*forkR);
    char newFork[5];
    intToString(phylosofersCount-1,newFork);
    (*forkR) = sem_open(newFork,1);
}

void closePhylosfers(int semId){
    for (int i = 0; i < phylosofersCount; i++) {
        phylosofers[i].isAlive = DEAD;
        sem_close(forks[i]);
    }
    sem_close(semId);
}

void addPhylosopher() {
    addFork(phylosofersCount);
    launchPhylosofer(phylosofersCount);
    phylosofers[phylosofersCount-1].isAlive = REFRESHADD;
    phylosofersCount++;
}

void removePhylosofer() {
    phylosofersCount--;
    phylosofers[phylosofersCount].isAlive = DEAD;
    sem_close(forks[phylosofersCount]);
    phylosofers[phylosofersCount-1].isAlive = REFRESHREM;
}

void addFork(int number) {
    char name[5];
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
    phylosofers[number].pid = createProcess(phylosofer,0,args);
    phylosofers[number].state = WAITING;
    phylosofers[number].isAlive = ALIVE;
}
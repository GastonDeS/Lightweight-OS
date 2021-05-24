#include <phylo.h>

typedef enum {WAITING,EATING}State; 
typedef enum {ALIVE,DEAD}IsAlive;

typedef struct {
    uint64_t pid;
    State state;
    IsAlive isAlive;
}Phylosofer;

int phylosofersCount = INITIALPHYLOSOFERS;
Phylosofer phylosofers[MAXPHYLOS];

void launchPhylosofer(int number);
void admin(int semId);
void removePhylosofer();
void addPhylosopher();
void closePhylosfers(int semId);
void printPhylos();
void addPhylos();
void printInst();

#define RIGHT(i) ((i) + 1) % (phylosofersCount)                         /* number of i’s right neighbor */
#define LEFT(i) ((i) + phylosofersCount - 1) % (phylosofersCount) /* number of i’s left neighbor */

int wantsToEat(int phyloN) {
    if (phylosofers[RIGHT(phyloN)].state == EATING || phylosofers[LEFT(phyloN)].state == EATING)
        return 0;
    return 1;
}

void leaveFork(int phyloN){
    phylosofers[phyloN].state = WAITING;
}

void phylosofer(int argc, char **argv){
    int semId = sem_open(SEMPHYLO,1);
    int phyloN = atoi(argv[1]);

    while (phylosofers[phyloN].isAlive!=DEAD || phylosofers[phyloN].state==EATING) {
        sem_wait(semId);
        if (phylosofers[phyloN].state) {
            leaveFork(phyloN);
        } else {
            if (wantsToEat(phyloN)) {
                phylosofers[phyloN].state = EATING;
                printPhylos();
            }
        }
        sem_post(semId);
        yieldSyscall();
    }
    sem_close(semId);
    myExit();
}

void phyloMaster(int argc, char **argv) {
    phylosofersCount = INITIALPHYLOSOFERS;
    int semId = sem_open(SEMPHYLO,1);
    printInst();
    sem_wait(semId);
    addPhylos();
    sem_post(semId);
    
    admin(semId);
    sem_close(semId);
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
            } else if (buf[0] == 'a' ){
                if (phylosofersCount<MAXPHYLOS) {
                    print(STDOUT, "A new phylosophers join. You have %d philosophers now\n",phylosofersCount+1);
                    addPhylosopher();
                } else 
                    print(STDOUT, "The table is full you can\'t add more than %d philosophers.\n",MAXPHYLOS);
            } else if (buf[0] == 'r' ) {
                if (phylosofersCount>MINPHYLOS) {
                    print(STDOUT, "You remove one philosopher of the problem, %d left\n",phylosofersCount-1);
                    removePhylosofer();
                } else {
                    print(STDOUT, "Can't leave only one philosopher he will be sad\n");
                }
            }
        }
    }
}

void printInst() {
    print(STDOUT, "Welcome to the philosophers pronlem!\n");
    print(STDOUT, "You are going to start with %d philosophers\nYou have a maximun of %d philosophers and have a minimun of %d\n", phylosofersCount, MAXPHYLOS, MINPHYLOS);
    print(STDOUT, "You can add then with \'a\', delete them with \'d\' and exit with \'e\'.\n");
    print(STDOUT, "The state of each will be displayed as E (EATING) or . (HUNGRY)\n");
}

void printPhylos(){ 
    for (int i = 0; i < phylosofersCount; i++) {
        if (phylosofers[i].state==EATING)
            print(STDOUT, "E");
        else
            print(STDOUT, ".");
    }
    print(STDOUT, "\n");
}

void closePhylosfers(int semId){
    for (int i = 0; i < phylosofersCount; i++) {
        phylosofers[i].isAlive = DEAD;
    }
}

void addPhylosopher() {
    launchPhylosofer(phylosofersCount);
    phylosofersCount++;
}

void addPhylos() {
    for (int i = 0; i < phylosofersCount; i++) {
        launchPhylosofer(i);
    }
}

void removePhylosofer() {
    phylosofersCount--;
    phylosofers[phylosofersCount].isAlive = DEAD;
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
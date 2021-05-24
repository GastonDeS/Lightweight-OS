#include <processCom.h>

int notAVocal(char c);
int isPipe(int pipeId);
void pipesOpen(int pipeId1, int pipeId2);
void pipesClose(int pipeId1, int pipeId2);

void cat(int argc, char **argv) {
    int pipeRead = atoi(argv[1]);    
    int pipeWrite = atoi(argv[2]);
    pipesOpen(pipeRead, pipeWrite);

    char c;
    int i = 0;
    char toPrint[1024] = {0};
    while ((c = getChar(pipeRead)) != EOF) {
        if(!isPipe(pipeRead))
            putChar(-1, c);
        toPrint[i++] = c;
        if (c == '\n') {
            toPrint[i] = 0;
            print(pipeWrite, "%s", toPrint);
            i = 0;
        }
    }
    pipesClose(pipeRead, pipeWrite);
    myExit();
}

void wc(int argc, char **argv) {
    int pipeRead = atoi(argv[1]);    
    int pipeWrite = atoi(argv[2]);
    pipesOpen(pipeRead, pipeWrite);

    uint32_t lines = 0;
    char c;
    while ((c = getChar(pipeRead)) != EOF) {
        if (!isPipe(pipeRead)) {
            putChar(-1, c);
        }
        if (c == '\n') {
            lines++;
        }
    }
    print(pipeWrite,"lines = %d", lines);
    pipesClose(pipeRead, pipeWrite);
    myExit();
}

void filter(int argc, char **argv) {
    int pipeRead = atoi(argv[1]);    
    int pipeWrite = atoi(argv[2]);
    pipesOpen(pipeRead, pipeWrite);

    char toPrint[1024] = {0};
    int i = 0;
    char c;
    while ((c = getChar(pipeRead)) != EOF) {
        if (!isPipe(pipeRead)) {
            putChar(-1, c);
        }
        if (notAVocal(c)) {
            toPrint[i++] = c;
        }
        if (c == '\n') {
            toPrint[i] = 0;
            print(pipeWrite, "%s", toPrint);
            i = 0;
        }
    }
    pipesClose(pipeRead, pipeWrite);
    myExit();
}

int notAVocal(char c) {
    return (c != 'a' && c !='e' && c != 'i' && c != 'o' && c != 'u' && c != 'A' && c !='E' && c != 'I' && c != 'O' && c != 'U');
}

int isPipe(int pipeId) {
    return pipeId >= 0;
}

void pipesOpen(int pipeId1, int pipeId2) {
    if(isPipe(pipeId1)) {
        pipeOpen(pipeId1);
    }
    if(isPipe(pipeId2)) {
        pipeOpen(pipeId2);
    }
}

void pipesClose(int pipeId1, int pipeId2) {
    if(isPipe(pipeId1)) {
        pipeClose(pipeId1);
    }
    if(isPipe(pipeId2)) {
        pipeClose(pipeId2);
    }
}
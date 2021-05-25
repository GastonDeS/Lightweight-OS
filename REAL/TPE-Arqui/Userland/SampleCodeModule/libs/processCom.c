#include <processCom.h>

int notAVocal(char c);

void cat(int argc, char **argv) {
    int pipeR = atoi(argv[1]);    
    int pipeW = atoi(argv[2]);
    pipesOpen(pipeR, pipeW);

    char c;
    int i = 0;
    char toPrint[1024] = {0};
    int flagEnd = 1;
    while (flagEnd) {
        c = getChar(pipeR);
        if (c == EOF) {
            flagEnd = 0;
        }
        if(!isPipe(pipeR))
            putChar(-1, c);
        if (flagEnd) {
            toPrint[i++] = c;
        }
        if (c == '\n' || c==EOF) {
            toPrint[i] = 0;
            print(pipeW, "%s", toPrint);
            i = 0;
        }
    }
    putChar(pipeW, EOF);
    pipesClose(pipeR, pipeW);
    myExit();
}

void wc(int argc, char **argv) {
    int pipeR = atoi(argv[1]);    
    int pipeW = atoi(argv[2]);
    pipesOpen(pipeR, pipeW);
    int flagEnd = 1;

    uint32_t lines = 1;
    char c;
    while (flagEnd) {
        c = getChar(pipeR);
        if (c == EOF) {
            flagEnd = 0;
        }
        if (!isPipe(pipeR)) {
            putChar(-1, c);
        }
        if (c == '\n') {
            lines++;
        }
    }
    print(pipeW,"lines = %d \n", lines);
    putChar(pipeW, c);
    pipesClose(pipeR, pipeW);
    myExit();
}

void filter(int argc, char **argv) {
    int pipeR = atoi(argv[1]);    
    int pipeW = atoi(argv[2]);
    pipesOpen(pipeR, pipeW);
    int flagEnd = 1;

    char toPrint[1024] = {0};
    int i = 0;
    char c;
    while (flagEnd) {
        c = getChar(pipeR);
        if (c == EOF) {
            flagEnd = 0;
        }
        if (!isPipe(pipeR)) {
            putChar(-1, c);
        }
        if (notAVocal(c) && flagEnd) {
            toPrint[i++] = c;
        }
        if (c == '\n' || c == EOF) {
            toPrint[i] = 0;
            print(pipeW, "%s", toPrint);
            i = 0;
        }
    }
    putChar(pipeW, EOF);
    pipesClose(pipeR, pipeW);
    myExit();
}

void loop(int argc, char **argv) {
    int pipeR = atoi(argv[1]);    
    int pipeW = atoi(argv[2]);
    pipesOpen(pipeR, pipeW);
    putChar(pipeW, '\n');
    uint64_t i;
    while(1) {
        print(pipeW, "Pid: %d ", getPid() );
        for (i = 0; i < 200000000; i++){
        }
    }
    myExit();
}

int notAVocal(char c) {
    return (c != 'a' && c !='e' && c != 'i' && c != 'o' && c != 'u' && c != 'A' && c !='E' && c != 'I' && c != 'O' && c != 'U');
}
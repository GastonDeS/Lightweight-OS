#include <processCom.h>
#include <stdint.h>
#include <unistd.h>

int notAVocal(char c);

void cat(int argc, char **argv) {
    char c;
    int i = 0;
    char toPrint[512] = {0};
    while ((c = getChar()) != 'Q') {
        putChar(c);
        toPrint[i++] = c;
        if (c == '\n') {
            toPrint[i] = 0;
            print("%s", toPrint);
            i = 0;
        }
    }
    myExit();
}

void wc(int argc, char **argv) {
    uint32_t lines = 0;
    char c;
    while ((c = getChar()) != 'Q') {
        putChar(c);
        if (c == '\n') {
            lines++;
        }
    }
    print("lines = %d", lines);
    lines = 0;
    myExit();
}

void filter(int argc, char **argv) {
    char toPrint[512] = {0};
    int i = 0;
    char c;
    while ((c = getChar()) != 'Q') {
        putChar(c);
        if (notAVocal(c)) {
            toPrint[i++] = c;
        }
        if (c == '\n') {
            toPrint[i] = 0;
            print("%s", toPrint);
            i = 0;
        }
    }
    myExit();
}

int notAVocal(char c) {
    return (c != 'a' && c !='e' && c != 'i' && c != 'o' && c != 'u' && c != 'A' && c !='E' && c != 'I' && c != 'O' && c != 'U');
}
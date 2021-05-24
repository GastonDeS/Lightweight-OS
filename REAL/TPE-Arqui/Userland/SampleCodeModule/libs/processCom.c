#include <processCom.h>
#include <stdint.h>

int notAVocal(char c);

void catS(int argc, char **argv) {
    char c;
    int i = 0;
    char toPrint[512] = {0};
    while ((c = getChar()) != 'Q') {
        putChar(c);
        toPrint[i++] = c;
        if (c == '\n') {
            print("%s", toPrint);
            i = 0;
            while (toPrint[i] != 0) {
                toPrint[i++] = 0;
            }
            i = 0;
        }
    }
    myExit();
}

void wcS(int argc, char **argv) {
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

void filterS(int argc, char **argv) {
    char toPrint[512] = {0};
    int i = 0;
    char c;
    while ((c = getChar()) != 'Q') {
        putChar(c);
        if (notAVocal(c)) {
            toPrint[i++] = c;
        }
        if (c == '\n') {
            print("%s", toPrint);
            i = 0;
            while (toPrint[i] != 0) {
                toPrint[i++] = 0;
            }
            i = 0;
        }
    }
    myExit();
}

int notAVocal(char c) {
    return (c != 'a' && c !='e' && c != 'i' && c != 'o' && c != 'u' && c != 'A' && c !='E' && c != 'I' && c != 'O' && c != 'U');
}
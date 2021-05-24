#include "processCom.h"
#include <stdint.h>


void interCat(int argc, char **argv) {
    char c;
    while ((c = getChar()) != "Q") {
        putChar(c);
    }
    myExit();
}

void interWc(int argc, char **argv) {
    uint32_t lines = 0;
    char c;
    while ((c = getChar()) != "Q") {
        if (c == "\n") {
            lines++;
        }
    }
    print("lines %s", lines);
    myExit();
}

void interFilter(int argc, char **argv) {
    uint32_t lines = 0;
    char c;
    while ((c = getChar()) != "Q") {
        if (c != "a" && c !="e" && c != "i" && c != "o" && c != "u" && c != "A" && c !="E" && c != "I" && c != "O" && c != "U") {
            putChar(c);
        }
    }
    myExit();
}
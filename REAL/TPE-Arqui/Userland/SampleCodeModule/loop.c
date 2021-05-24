#include <loop.h>
#include <commands.h>
#include <stdint.h>

#define EOF -1

void *loop() {
    putChar('\n');
    uint64_t pid;
    uint64_t i;
    char c;
    while(1) {
        getPid(&pid);
        for (i = 0; i < 200000000; i++){
            //nada, es como un wait
        }
    }
    myExit();
}
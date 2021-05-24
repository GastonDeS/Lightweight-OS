#include <loop.h>
#include <commands.h>
#include <stdint.h>

void *loop() {
    putChar('\n');
    char *argv[2];
	argv[0] = "loop";
	argv[1] = 1;
    uint64_t i;
    while(1) {
        getPid(argv);
        for (i = 0; i < 200000000; i++){
            //nada, es como un wait
        }
    }
    myExit();
}
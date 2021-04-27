/* sampleCodeModule.c */
#include <stdio.h>
#include <Shell.h>
#include <syscallsASM.h>
#include <stdint.h>
#include <unistd.h>
#include <chess.h>

#define NULL (void *) 0

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	*v = 'X';
	*(v+1) = 0x74;

	char **argv = NULL;
	createProcess(init_shell,argv);

	drawRect(0,0,100,100,0xff0000);


	while (1)
	{
		/* code */
	}
	


	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

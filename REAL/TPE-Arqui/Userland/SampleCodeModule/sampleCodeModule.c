/* sampleCodeModule.c */
#include <stdio.h>
#include <Shell.h>
#include <syscallsASM.h>
#include <stdint.h>
char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	//All the following code may be removed
	*v = 'X';
	*(v+1) = 0x74;

	init_shell();

	uint64_t error = getError();
	if (error < 32) {
		uint64_t registers[19];
		getRegistersSyscall(registers);
		print("ERROR: EXCEPTION %x ", error);
		switch (error) {
			case 0: print("(DIVISION BY ZERO)\n");
			break;
			case 1: print("(INVALID OPCODE)\n");
		}
		print("REGISTERS STATUS:\n");
		print("R15: %X - R14: %X\n", registers[18], registers[17]);
	  print("R13: %X - R12: %X\n", registers[16], registers[15]);
	  print("R11: %X - R10: %X\n", registers[14], registers[13]);
	  print("R9: %X - R8: %X\n", registers[12], registers[11]);
	  print("RSI: %X - RDI: %X\n", registers[10], registers[9]);
	  print("RBP: %X - RDX: %X\n", registers[8], registers[7]);
	  print("RCX: %X - RBX: %X\n", registers[6], registers[5]);
	  print("RAX: %X - RIP: %X\n", registers[4], registers[3]);
	  print("CS: %X - FLAGS: %X\n", registers[2], registers[1]);
	  print("RSP: %X\n", registers[0]);
	} else {
		print("What module would you like to run? (type \"help\" to see commands)\n");
	}

	while (1) {
		int ch = getChar();
		keyPressedShell(ch);

	}

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

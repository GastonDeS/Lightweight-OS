#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define TOTAL_REGISTERS 17

// typedef union registerU{ //Es una struct y un vector al mismo tiempo, asi se puede iterar
// 	struct registersS {
// 		uint64_t r15;
// 		uint64_t r14;
// 		uint64_t r13;
// 		uint64_t r12;
// 		uint64_t r11;
// 		uint64_t r10;
// 		uint64_t r9;
// 		uint64_t r8;
// 		uint64_t rsi;
// 		uint64_t rdi;
// 		uint64_t rbp;
// 		uint64_t rdx;
// 		uint64_t rcx;
// 		uint64_t rbx;
// 		uint64_t rax;
// 		uint64_t rsp;
// 		uint64_t rip;
// 	};
// 	uint64_t registersV[TOTAL_REGISTERS];
// } registerStruct;

typedef struct registers {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t cs;
	uint64_t flags;
	uint64_t rsp;
	uint64_t rip;
	// uint64_t rsp;
	// uint64_t rip;
} registerStruct;


#endif

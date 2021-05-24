#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#define CTRL_L 29
#define SHIFT_L 42
#define SHIFT_R 54


#define BLOCK_M 58
#define F1 59

#define KEY_RELEASED 128
#define KEYS 59

#include <regi.h>
#include <stdint.h>

void keyboardHandler(registerStruct *);
void readKeyboard(char * buff, uint8_t size, uint64_t * count);
void bufferEmpty(uint64_t * target);
void isMayus(uint64_t * target);
#endif

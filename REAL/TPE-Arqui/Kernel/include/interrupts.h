#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void _cli();
void _sti();
void picSlaveMask(uint8_t mask);
void picMasterMask(uint8_t mask);

void _irq00Handler(void);
void _irq01Handler(void);
void _irq80Handler(void);
void _exc00Handler(void);
void _exc06Handler(void);

void saveInitialConditions(void* address);

#endif

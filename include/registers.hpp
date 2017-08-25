#ifndef REGISTERS_H
#define REGISTERS_H

#include "constants.hpp"

#define flag_c (1 << 4)
#define flag_h (1 << 5)
#define flag_n (1 << 6)
#define flag_z (1 << 7)

struct Registers
{
	Word af; // union of a and f
	Word bc; // union of b and c
	Word de; // union of d and e
	Word hl; // union of h and l

	WORD sp; // stack pointer
	WORD pc; // program counter (PC)
};

extern bool flagCarry(Registers& r);

extern bool flagHalf(Registers& r);

extern bool flagNegative(Registers& r);

extern bool flagZero(Registers& r);

extern void flagSet(Registers& r, BYTE f);

extern void flagClear(Registers& r, BYTE f);

#endif
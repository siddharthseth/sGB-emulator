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

bool flagCarry(Registers& r) {
	return r.af.b.b2 & flag_c;
}

bool flagHalf(Registers& r) {
	return r.af.b.b2  & flag_h;
}

bool flagNegative(Registers& r) {
	return r.af.b.b2 & flag_n;
}

bool flagZero(Registers& r) {
	return r.af.b.b2 & flag_z;
}

void flagSet(Registers& r, BYTE f) {
	r.af.b.b2 |= f;
}

void flagClear(Registers& r, BYTE f) {
	r.af.b.b2 &= ~f;
}

#endif
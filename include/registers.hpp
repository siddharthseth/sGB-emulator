#ifndef REGISTERS_H
#define REGISTERS_H

#include "constants.hpp"

struct Registers
{
	union {
		struct {
			BYTE f, a;
		} _af;
		WORD af;
	}; // registers A + FLAG

	union {
		struct {
			BYTE c, b;
		} _bc;
		WORD bc;
	}; // registers B + C

	union {
		struct {
			BYTE e, d;
		} _de;
		WORD de;
	}; // registers D + E

	union {
		struct {
			BYTE l, h;
		} _hl;
		WORD hl;
	}; // registers H + L

	WORD sp; // stack pointer
	WORD pc; // program counter (PC)
};

#endif
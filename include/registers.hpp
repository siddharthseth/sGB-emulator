#ifndef REGISTERS_H
#define REGISTERS_H

#include "constants.hpp"

struct Registers
{
	union {
		struct {
			BYTE a, f;
		} _af;
		WORD af;
	}; // registers A + FLAG

	union {
		struct {
			BYTE b, c;
		} _bc;
		WORD bc;
	}; // registers B + C

	union {
		struct {
			BYTE d, e;
		} _de;
		WORD de;
	}; // registers D + E

	union {
		struct {
			BYTE h, l;
		} _hl;
		WORD hl;
	}; // registers H + L

	WORD sp; // stack pointer
	WORD pc; // program counter (PC)
};

#endif
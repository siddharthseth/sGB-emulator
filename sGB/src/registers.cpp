#include "registers.hpp"

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
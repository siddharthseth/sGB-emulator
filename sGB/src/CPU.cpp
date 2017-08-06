#include "CPU.hpp"

using namespace std;

CPU::CPU() : 
registers(),
mmu(),
clock()
{
	reset();
}

bool CPU::step()
{
	// Fetch next instruction & increment counter
	BYTE instr = mmu.readByte(registers.pc++);

	// Decode instruction

	// Execute instruction
}

void CPU::reset()
{
	registers.af = 0;
	registers.bc = 0;
	registers.de = 0;
	registers.hl = 0;
	registers.sp = 0;
	registers.pc = 0;


}
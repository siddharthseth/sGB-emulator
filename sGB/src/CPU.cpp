#include "CPU.hpp"
#include <iostream>

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
	BYTE instr = mmu->readByte(registers->pc++);

	// Decode instruction
	struct instruction instruction = instructionsTable[instr];

	// Execute instruction
	if (instruction.func != NULL) 
	{
		return true;
	} 
	else 
	{
		cout << "Instruction '" << instr << "' not implemented." << endl;
		cout << "Description: " << instruction.description << endl; 
		return false;
	}
}

void CPU::reset()
{
	registers->af = 0;
	registers->bc = 0;
	registers->de = 0;
	registers->hl = 0;
	registers->sp = 0;
	registers->pc = 0;
}
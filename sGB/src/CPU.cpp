#include "CPU.hpp"
#include <iostream>
#include <cstring>

using namespace std;

CPU::CPU() : 
registers(new Registers()),
mmu(new MMU()),
clock(new Clock())
{
	reset();
}

int CPU::step()
{
	// Fetch next instruction & increment counter
	BYTE instr = mmu->readByte(registers->pc++);

	// Decode instruction
	struct instruction instruction = instructionsTable[instr];

	// Execute instruction
	if (instruction.func != NULL){
		switch(instruction.operandLength)
		{
			case 0:
				(this->*(instruction.func))(0);
				break;
			case 1:
				(this->*(instruction.func))(mmu->readByte(registers->pc));
				break;
			case 2:
				(this->*(instruction.func))(mmu->readWord(registers->pc));
				break;
		}

		registers->pc += instruction.operandLength;
		clock->updateClocks(instruction.cycles);
		return instruction.cycles;
	} else
	{
		cout << "PC: " << hex((registers->pc - 1) >> 8) << hex(registers->pc - 1) <<  endl;
		cout << "Instruction '" << instruction.assembly << "' not implemented." << endl;
		cout << "Description: " << instruction.description << endl; 
		return -1;
	}
}

void CPU::loadROM(ifstream &romFile)
{
	char cartridgeInfo[0x014F];
	romFile.read(cartridgeInfo, 0x014F);

	BYTE romTypeVal = cartridgeInfo[ROM_TYPE_ADDRESS];
	romType = romTypeNames[romTypeVal];
	cout << "ROM type: " << romType << endl;

	BYTE romSizeVal = cartridgeInfo[ROM_SIZE_ADDRESS];
	cout << "ROM size: " << hex(romSizeVal) << endl;

	BYTE ramSizeVal = cartridgeInfo[RAM_SIZE_ADDRESS];
	cout << "RAM size: " << hex(ramSizeVal) << endl;

	char romName[16];
	memset(romName, 0, 16);
	for(int i = 0; i < 16; i++) {
		romName[i] = cartridgeInfo[ROM_NAME_ADDRESS + i];
	}
	cout << "ROM Name: " << romName << endl;

	romFile.seekg(0, romFile.beg);
	mmu->loadGame(romFile, romTypeVal);
}

/*
 * Gameboy is reset to the same state everytime according to specs...
 */
void CPU::reset()
{
	mmu->reset();

	registers->af.w = 0x01B0;
	registers->bc.w = 0x0013;
	registers->de.w = 0x00D8;
	registers->hl.w = 0x014D;

	registers->pc = 0x0100;
	registers->sp = 0xFFFE;

	clock->resetClocks();
}

void CPU::writeStack(WORD data)
{
	registers->sp -= 2;
	mmu->writeWord(registers->sp, data);
}

void CPU::add(BYTE& dest, BYTE value)
{
	flagClear(*registers, flag_n);
	// use int incase of carry, if upper 8 bits are set
	unsigned int sum = dest + value;
	dest = (BYTE) sum & 0xff; // only want lower 8 bits

	if(sum & 0xff00) {
		flagSet(*registers, flag_c);
	} else {
		flagClear(*registers, flag_c);
	}

	if((dest & 0x0f) + (value & 0x0f) > 0x0f) {
		flagSet(*registers, flag_h);
	} else {
		flagClear(*registers, flag_h);
	}

	if(dest) {
		flagClear(*registers, flag_z);
	} else {
		flagSet(*registers, flag_z);
	}
}

void CPU::addWord(WORD& dest, WORD value)
{
	flagClear(*registers, flag_n);

	unsigned long sum = dest + value;
	dest = (WORD) sum & 0xffff;

	if(sum & 0xffff0000) {
		flagSet(*registers, flag_c);
	} else {
		flagClear(*registers, flag_c);
	}

	if((dest & 0x00ff) + (value & 0x00ff) > 0x00ff) {
		flagSet(*registers, flag_h);
	} else {
		flagClear(*registers, flag_h);
	}
}


BYTE CPU::increment(BYTE value)
{
	flagClear(*registers, flag_n); // reset neg
	
	bool halfCarry = value & 0x0f;
	if(halfCarry) {
		flagSet(*registers, flag_h);
	} else {
		flagClear(*registers, flag_h);
	}

	value++;

	if(value) {
		flagClear(*registers, flag_z);
	} else {
		flagSet(*registers, flag_z);
	}

	return value;
}

BYTE CPU::decrement(BYTE value)
{
	flagSet(*registers, flag_n);

	bool halfCarry = value & 0x0f;
	if(halfCarry) {
		flagClear(*registers, flag_h);
	} else {
		flagSet(*registers, flag_h);
	}

	value--;

	if(value) {
		flagClear(*registers, flag_z);
	} else {
		flagSet(*registers, flag_z);
	}

	return value;
}
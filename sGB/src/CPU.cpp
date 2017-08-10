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

	registers->pc = 0x0100;
	registers->af = 0x01B0;
	registers->bc = 0x0013;
	registers->de = 0x00D8;
	registers->hl = 0x014D;
	registers->sp = 0xFFFE;

	clock->resetClocks();
}
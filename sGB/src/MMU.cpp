#include "MMU.hpp"
#include <cstring>
#include <iostream>

using namespace std;

MMU::MMU():
currROMBank(1),
currRAMBank(0)
{
}

/*
 * Read an 8-bit section from memory
*/
BYTE MMU::readByte(WORD address)
{
	if (address < 0x8000)
	{
		return cartridge[address];
	}
	else if (0x8000 <= address && address < 0xa000)
	{
		return vram[address - 0x8000];
	}
	else if (0xa000 <= address && address < 0xc000)
	{
		return xram[address - 0xa000];
	}
	else if (0xc000 <= address && address < 0xe000)
	{
		return wram[address - 0xc000];
	}
	else if (0xe000 <= address && address < 0xfe00)
	{
		return wram[address - 0xe000];
	}
	else if (0xfe00 <= address && address < 0xff00)
	{
		// first part is accessible for oam, rest is empty
		if (address < 0xfea0) 
		{
			return oam[address - 0xfe00];	
		} else
		{
			return 0;
		}
	}
	else if (0xff00 <= address && address < 0xff80)
	{
		if (address < 0xff4c)
		{
			return io[address - 0xff00];
		} else 
		{
			return 0;
		}
	} 
	else if (0xff80 <= address && address < 0xffff)
	{
		return ram[address - 0xff80];
	}
	else 
	{
		return 0;
	}
}

/*
 * Read a 16-bit word from memory
*/
WORD MMU::readWord(WORD address)
{
	return readByte(address) | (readByte(address + 1) << 8);
}

void MMU::writeByte(WORD address, BYTE data)
{
	// First 0x8000 is cartridge memory, and read only...
	if (address < 0x8000)
		return;

	if (address < 0xa000)
	{
		vram[address - 0x8000] = data;
		return;
	}

	if (address < 0xc000)
	{
		xram[address - 0xa000] = data;
		return;
	}

	if (address < 0xe000)
	{
		wram[address - 0xc000] = data;
		return;
	}

	if (address < 0xfe00)
	{
		wram[address - 0xe000] = data;
		return;

	}
	
	if (address < 0xff00)
	{
		// first part is accessible for oam, rest is empty
		if (address < 0xfea0) 
		{
			oam[address - 0xfe00] = data;
		}
		return;
	}

	if (address < 0xff80)
	{
		if (address < 0xff4c)
		{
			io[address - 0xff00] = data;
		}
		return;
	} 
	
	if (address < 0xffff)
	{
		ram[address - 0xff80] = data;
	}
}

void MMU::writeWord(WORD address, WORD data)
{
	this->writeByte(address, data & 0xff);
	this->writeByte(address + 1, (data >> 8) & 0xff);
}

void MMU::reset()
{
	cout << "Currently reseting all memory..." << endl;
	// reset all memory to zero
	memset(cartridge, 0, sizeof(cartridge));
	memset(vram, 0, sizeof(vram));
	memset(xram, 0, sizeof(xram));
	memset(wram, 0, sizeof(wram));
	memset(oam, 0, sizeof(oam));
	memset(io, 0, sizeof(io)); // might have to set some io defaults instead
	memset(ram, 0, sizeof(ram));

	// set some required memory accordingly
	writeByte(0xFF05, 0x00); 
	writeByte(0xFF06, 0x00); 
	writeByte(0xFF07, 0x00); 
	writeByte(0xFF10, 0x80); 
	writeByte(0xFF11, 0xBF); 
	writeByte(0xFF12, 0xF3); 
	writeByte(0xFF14, 0xBF); 
	writeByte(0xFF16, 0x3F); 
	writeByte(0xFF17, 0x00); 
	writeByte(0xFF19, 0xBF); 
	writeByte(0xFF1A, 0x7F); 
	writeByte(0xFF1B, 0xFF); 
	writeByte(0xFF1C, 0x9F); 
	writeByte(0xFF1E, 0xBF); 
	writeByte(0xFF20, 0xFF); 
	writeByte(0xFF21, 0x00); 
	writeByte(0xFF22, 0x00); 
	writeByte(0xFF23, 0xBF); 
	writeByte(0xFF24, 0x77); 
	writeByte(0xFF25, 0xF3);
	writeByte(0xFF26, 0xF1); 
	writeByte(0xFF40, 0x91); 
	writeByte(0xFF42, 0x00); 
	writeByte(0xFF43, 0x00); 
	writeByte(0xFF45, 0x00); 
	writeByte(0xFF47, 0xFC); 
	writeByte(0xFF48, 0xFF); 
	writeByte(0xFF49, 0xFF); 
	writeByte(0xFF4A, 0x00); 
	writeByte(0xFF4B, 0x00); 
	writeByte(0xFFFF, 0x00); 

	cout << "Successfully reset all memory!" << endl;
}

void MMU::loadGame(ifstream& romFile, BYTE romTypeVal)
{
	romType rt = static_cast<romType>(romTypeVal);

	switch(rt) {
		case ROM_ONLY:
			romFile.read((char*) cartridge, 0x8000);
			break;
		case ROM_MBC1:
		case ROM_MBC1_RAM:
		case ROM_MBC1_RAM_BATT:
		case ROM_MBC2:
		case ROM_MBC2_BATTERY:
		case ROM_RAM:
		case ROM_RAM_BATTERY:
		case ROM_MMM01:
		case ROM_MMM01_SRAM:
		case ROM_MMM01_SRAM_BATT:
		case ROM_MBC3_TIMER_BATT:
		case ROM_MBC3_TIMER_RAM_BATT:
		case ROM_MBC3:
		case ROM_MBC3_RAM:
		case ROM_MBC3_RAM_BATT:
		case ROM_MBC5:
		case ROM_MBC5_RAM:		
		case ROM_MBC5_RAM_BATT:
		case ROM_MBC5_RUMBLE:
		case ROM_MBC5_RUMBLE_SRAM:
		case ROM_MBC5_RUMBLE_SRAM_BATT:
		case ROM_POCKET_CAMERA:
		case ROM_BANDAI_TAMA5:
		case ROM_HUDSON_HUC3:
		case ROM_HUDSON_HUC1:
			break;
	}
}
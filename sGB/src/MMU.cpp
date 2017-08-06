#include "MMU.hpp"

using namespace std;

MMU::MMU()
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
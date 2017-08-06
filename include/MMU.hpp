#ifndef MMU_H
#define MMU_H

#include "constants.hpp"

/* MEMORY MODEL:

 Interrupt Enable Register
 --------------------------- FFFF
 Internal RAM
 --------------------------- FF80
 Empty but unusable for I/O
 --------------------------- FF4C
 I/O ports
 --------------------------- FF00
 Empty but unusable for I/O
 --------------------------- FEA0
 Sprite Attrib Memory (OAM)
 --------------------------- FE00
 Echo of 8kB Internal RAM
 --------------------------- E000
 8kB Internal RAM
 --------------------------- C000
 8kB switchable RAM bank
 --------------------------- A000
 8kB Video RAM
 --------------------------- 8000 --
 16kB switchable ROM bank |
 --------------------------- 4000 |= 32kB Cartrigbe
 16kB ROM bank #0 |
 --------------------------- 0000 --

*/

class MMU
{
	public:
		MMU();
		virtual ~MMU() {};

		BYTE readByte(WORD);
		WORD readWord(WORD);

	private:
		BYTE cartridge[0x8000];
		BYTE vram[0x2000];
		BYTE xram[0x2000];
		BYTE wram[0x2000];
		BYTE oam[0x100];
		BYTE io[0x80];
		BYTE ram[0x80];
};

#endif
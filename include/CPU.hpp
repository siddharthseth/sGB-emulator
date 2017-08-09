#ifndef CPU_H
#define CPU_H

#include <memory>
#include <string>
#include <fstream>
#include "clock.hpp"
#include "registers.hpp"
#include "MMU.hpp"
#include "rom.hpp"

using namespace std;

class CPU 
{
	public:
		CPU();
		virtual ~CPU() {};

		int step();
		void reset();
		void loadROM(ifstream&);

	private:
		unique_ptr<Registers> registers;
		unique_ptr<MMU> mmu;
		unique_ptr<Clock> clock;

		// represents type of rom
		string romType;
};

#endif
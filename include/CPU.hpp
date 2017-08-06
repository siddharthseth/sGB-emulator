#ifndef CPU_H
#define CPU_H

#include <memory>
#include "clock.hpp"
#include "registers.hpp"
#include "MMU.hpp"

class CPU 
{
	public:
		CPU();
		virtual ~CPU() {};

		bool step();
	private:
		std::unique_ptr<Registers> registers;
		std::unique_ptr<MMU> mmu;
		std::unique_ptr<Clock> clock;
};

#endif
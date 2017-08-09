#include "clock.hpp"

using namespace std;

Clock::Clock() :
machineCycles(0),
clockCycles(0)
{

}

void Clock::updateClocks(int cycles)
{
	clockCycles += cycles;
	machineCycles += (cycles/4);
}

void Clock::resetClocks()
{
	clockCycles = 0;
	machineCycles = 0;
}
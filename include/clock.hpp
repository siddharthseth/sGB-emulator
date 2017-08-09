#ifndef CLOCK_H
#define CLOCK_H

class Clock
{
	public:
		Clock();
		virtual ~Clock() {};

		// cycles in clock cycles
		void updateClocks(int cycles);
		void resetClocks();

	private:
		// 1 machine cycle == 4 clock cycles
		unsigned int machineCycles;
		unsigned int clockCycles;
};

#endif
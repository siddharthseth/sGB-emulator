#ifndef CLOCK_H
#define CLOCK_H

class Clock
{
	public:
		Clock();
		virtual ~Clock();

	private:
		unsigned int machineCycles;
		unsigned int clockCycles;
};

#endif
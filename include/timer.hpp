#ifndef TIMER_H
#define TIMER_H

#include "constants.hpp"

class Timer
{
	public:
		Timer();
		virtual ~Timer() {};

		void step(int);
	private:
};

#endif
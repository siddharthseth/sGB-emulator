#ifndef TIMER_H
#define TIMER_H

#include "constants.hpp"

class Timer
{
	public:
		Timer();
		virtual ~Timer() {};

		void step(int);
		void reset();
	private:
		int timerCounter;
		int dividerCounter;
		int frequency;

		void dividerRegister(int);
		bool isTimerEnabled();
		void setTimerFreq();
};

#endif
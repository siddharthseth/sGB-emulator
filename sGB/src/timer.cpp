#include "timer.hpp"
#include "CPU.hpp"

Timer::Timer()
{
	reset();
}

void Timer::step(int cycles)
{
	mmu->dividerRegister(cycles); 

	// the clock must be enabled to update the clock 
	if (isTimerEnabled())
	{ 
	 	timerCounter -= cycles; 

	  	// enough cpu clock cycles have happened to update the timer
	  	if (timerCounter <= 0)
		{
		    // reset m_TimerTracer to the correct value
		    SetClockFreq( );

	    // timer about to overflow
	    if (mmu->readByte(TIMA) == 255)
	    { 
	    	mmu->writeByte(TIMA, mmu->readByte(TMA));
	    	//RequestInterupt(2);
	    }
	    else
	    {
	    	mmu->writeByte(TIMA, mmu->readByte(TIMA)+1) ;
	    }
	  } 
	}
}

void Timer::reset()
{
	timerCounter = 1024;
	dividerCounter = 0;
}

bool Timer::isTimerEnabled()
{
	// bit 2 specifies if timer is enabled
	BYTE val = mmu->readByte(TMC);
	return ((val >> 2) & 1) ? true : false;
}

void Timer::setTimerFreq()
{
	BYTE freq = mmu->getTimerFreq();
	switch(freq)
	{
		case 0: timerCounter = 1024;
				break;
		case 1: timerCounter = 16;
				break;
		case 2: timerCounter = 64;
				break;
		case 3: timerCounter = 256;
				break;
	}
}
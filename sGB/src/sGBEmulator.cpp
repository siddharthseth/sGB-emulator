#include <iostream>
#include "constants.hpp"
#include "sGBEmulator.hpp"

using namespace std;

sGBEmulator::sGBEmulator(string romPath) : 
romPath(romPath), 
romFile(romPath.c_str(), ifstream::binary),
cpu(new CPU()),
gpu(new GPU()),
timer(new Timer())
{
	bool success = initialize();
	if (success) {
		cout << "Initialization succeeded!" << endl;
	} else {
		cout << "Initialization failed, stopping now..." << endl;
	}
}

bool sGBEmulator::initialize() 
{
	cout << "Attempting to load rom from: " << romPath << endl;

	if (romFile.is_open()) {
		cpu->loadROM(romFile);
		romFile.close();

		cout << "Successfully loaded rom!" << endl;
		return true;
	} else {
		cout << "Error opening from: " << romPath << endl;
		return false;
	}
}

bool sGBEmulator::update()
{
	int cyclesThisUpdate = 0;

	while (cyclesThisUpdate < MAXCYCLES)
	{
		int cycles = this->cpuStep();
		if (cycles == -1)
		{
			return false;
		}

		cyclesThisUpdate += cycles;
		this->timerStep(cycles);
		this->gpuStep(cycles);
		this->interruptStep();
	}

	// renderScreen();
	return true;
}

int sGBEmulator::cpuStep() 
{
	return cpu->step();
}

void sGBEmulator::timerStep(int cycles)
{
	timer->step(cycles);
}

void sGBEmulator::gpuStep(int cycles) 
{
	gpu->step(cycles);
}

void sGBEmulator::interruptStep() 
{

}
#include <iostream>
#include "constants.hpp"
#include "sGBEmulator.hpp"

using namespace std;

sGBEmulator::sGBEmulator(string romPath) : 
romPath(romPath), 
romFile(romPath.c_str()),
cpu()
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
	string line;
	cout << "Attempting to load rom from: " << romPath << endl;

	if (romFile.is_open()) {
		while ( getline(romFile, line) )
		{
			cout << line << endl;
		}
		romFile.close();

		cout << "Successfully loaded rom!" << endl;
		return true;
	} else {
		cout << "Error opening from: " << romPath << endl;
		return false;
	}
}

bool sGBEmulator::cpuStep() 
{
	return cpu->step();
}

bool sGBEmulator::gpuStep() 
{
	return true;
}

bool sGBEmulator::interruptStep() 
{
	return true;
}
#ifndef SGBEMULATOR_H
#define SGBEMULATOR_H

#include <memory>
#include <string>
#include <fstream>
#include "CPU.hpp"

class sGBEmulator
{
	public:
		sGBEmulator(std::string);
		virtual ~sGBEmulator() {};

		bool update();

	private:
		std::string romPath;
		std::ifstream romFile;
		std::unique_ptr<CPU> cpu;

		int cpuStep();
		bool gpuStep(int);
		bool interruptStep();

		bool initialize();

};

#endif
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
		bool cpuStep();
		bool gpuStep();
		bool interruptStep();

	private:
		std::string romPath;
		std::ifstream romFile;
		std::unique_ptr<CPU> cpu;


		bool initialize();

};

#endif
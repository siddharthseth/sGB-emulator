#ifndef SGBEMULATOR_H
#define SGBEMULATOR_H

#include <memory>
#include <string>
#include <fstream>
#include "timer.hpp"
#include "CPU.hpp"
#include "GPU.hpp"

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
		std::unique_ptr<GPU> gpu;
		std::unique_ptr<Timer> timer;

		int cpuStep();
		void timerStep(int);
		void gpuStep(int);
		void interruptStep();

		bool initialize();

};

#endif
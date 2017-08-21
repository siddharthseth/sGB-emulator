#include "GPU.hpp"

GPU::GPU():
scanningCounter(0),
gpuMode(HBLANK),
currLine(0)
{

}

// http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-GPU-Timings
GPU::step(int cycles)
{
	if (!isEnabled()) {
		return;
	}

	scanningCounter += cycles;

	switch(mode) {
		case HBLANK:
			if (scanningCounter >= 204) {
				currLine++;
				scanningCounter = 0;

				// reached last line, enter vblank
				if (line == 143) {
					mode = VBLANK;

				} else {
					mode = OAM;
				}
			}
			break;
		case VBLANK:

		case OAM:

		case VRAM:

	}
}
#ifndef GPU_H
#define GPU_H

class GPU
{
	public:
		GPU();
		virtual ~GPU() {};

		void step(int cycles);

	private:
		enum mode {
			HBLANK = 0,
			VBLANK = 1,
			OAM = 2,
			VRAM = 3
		};
		enum mode gpuMode;
		
		bool isEnabled();
		int scanningCounter;
		int currLine;
};

#endif
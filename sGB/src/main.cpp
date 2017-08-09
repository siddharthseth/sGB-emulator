#include <iostream>
#include <string>
#include <SDL.h>
#include "res_path.hpp"
#include "cleanup.hpp"
#include "sGBEmulator.hpp"

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
int WINDOW_SCALE = 2;

using namespace std;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(ostream &os, const string &msg){
	os << msg << " error: " << SDL_GetError() << endl;
}

int main (int argc, char** argv)
{
	// check for a filename of the game...
	if(argc < 2)
	{
		cout << "Error: input filename." << endl;
		return 1;
	}

	// check if specified a window scaling attribute, otherwise default is 2
	if(argc == 3) {
		WINDOW_SCALE = stoi(argv[2]);
	}

	string romName = argv[1];
	string romPath = getResourcePath("roms") + romName;

	// Initialize SDL Engine w/Video and Audio flags
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		logSDLError(cout, "SDL_Init");
	}

	// Initialize SDL Window and error check
	SDL_Window *pWindow = SDL_CreateWindow("sGB Emulator",
											SDL_WINDOWPOS_UNDEFINED,
											SDL_WINDOWPOS_UNDEFINED,
											SCREEN_WIDTH * WINDOW_SCALE,
											SCREEN_HEIGHT * WINDOW_SCALE,
											SDL_WINDOW_SHOWN);
	if (pWindow == nullptr)
	{
		logSDLError(cout, "SDL_CreateWindow");
		return 1;
	}

	// Initialize SDL Renderer and error check
	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pRenderer == nullptr)
	{
		logSDLError(cout, "SDL_CreateRenderer");
		return 1;
	}

	sGBEmulator sGB(romPath);

	SDL_Event e;
	bool running = true;

	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				running = false;
			}
		}

		bool success = sGB.update();
		if (!success)
		{
			running = false;;
		}
		
		SDL_RenderClear(pRenderer);

		SDL_RenderPresent(pRenderer);
	}

	cleanup(pRenderer, pWindow);
	SDL_Quit();
	return 0;
}
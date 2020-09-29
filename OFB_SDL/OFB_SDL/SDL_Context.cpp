#include "SDL_Context.h"

void windowContext::App::start(int width, int height) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	initWindow();
	mainLoop();
	cleanUp();
}

void windowContext::App::initWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		printf("SDL initialized. Creating window.\n");
		//Create window
		//window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		// Create a window
		SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
	}
}

void windowContext::App::mainLoop()
{

}

void windowContext::App::cleanUp()
{
	printf("Destroying instances and cleaning up.\n");

	//Destroy the renderer
	SDL_DestroyRenderer(renderer);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
}

int windowContext::App::getWidth()
{
	return SCREEN_WIDTH;
}

int windowContext::App::getHeight()
{
	return SCREEN_HEIGHT;
}
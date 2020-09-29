#pragma once

// Core Includes
#include <SDL.h>
#include <stdio.h>

namespace windowContext
{
	class App
	{
	private:
		//Screen dimension constants
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;

	public:
		// To keep the app alive
		bool keepAlive = true;

		// SDL specific variables
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;

		//┌──────────────────────────────────────────────────────────────────┐
		//│							Core Functions							 │
		//└──────────────────────────────────────────────────────────────────┘
		void start(int width, int height);
		void initWindow();
		virtual void mainLoop();
		void cleanUp();

		//┌──────────────────────────────────────────────────────────────────┐
		//│						   Utility Functions						 │
		//└──────────────────────────────────────────────────────────────────┘
		int getWidth();
		int getHeight();
	};
}

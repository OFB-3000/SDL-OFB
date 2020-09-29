/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include "SDL_Context.h"
#include <stdexcept>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

//Some useful globals
double posX = 5, posY = 5;  //x and y start position
double dirX = -1, dirY = 0; //initial direction vector
double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

double time = 0; //time of current frame
double oldTime = 0; //time of previous frame
//const int w = SCREEN_WIDTH;
//const int h = SCREEN_HEIGHT;

double oldDirX;
double oldPlaneX;

/*
class Game
{
public:
	void run(int width, int height)
	{
		initWindow(width, height);
		mainLoop();
		cleanUp();
	}


private:

	bool keepAlive = true;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	void initWindow(int width, int height)
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

	void mainLoop()
	{
		printf("Entering main loop\n");

		//timing for input and FPS counter
		oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds

		//speed modifiers
		double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
		double rotSpeed = frameTime * 3.0; //the constant value is in radians/second

		//Map dimensions
		const int mapWidth = 10;
		const int mapHeight = 10;

		const int worldMap[mapWidth][mapHeight] = {
			{1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,1,0,0,0,1},
			{1,0,0,0,0,1,1,1,0,1},
			{1,0,0,0,0,0,0,1,0,1},
			{1,0,1,0,0,0,0,1,0,1},
			{1,0,1,0,0,0,0,1,0,1},
			{1,0,1,0,0,0,0,0,0,1},
			{1,0,1,1,1,0,0,0,0,1},
			{1,0,0,0,1,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1}
		};

		while (keepAlive) {
			SDL_Event event;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);

			//Event polling
			while (SDL_PollEvent(&event) != 0) {
				//User requests quit
				if (event.type == SDL_QUIT)
				{
					printf("Exiting\n");
					keepAlive = false;
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_SPACE:
						printf("Space\n");
						break;

					case SDLK_ESCAPE:
						printf("Exiting\n");
						keepAlive = false;
						break;
					case SDLK_w:
						printf("Pressed W\n");
						if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
						if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
						break;
					case SDLK_s:
						printf("Pressed S\n");
						if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
						if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
						break;
					case SDLK_RIGHT:
						printf("Pressed ->\n");
						oldDirX = dirX;
						dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
						dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
						oldPlaneX = planeX;
						planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
						planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
						break;
					case SDLK_LEFT:
						printf("Pressed <-\n");
						oldDirX = dirX;
						dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
						dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
						oldPlaneX = planeX;
						planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
						planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
						break;
					default:
						break;
					}
				}
			}

			//Calculations
			for (int x = 0; x < w; x++)
			{
				//calculate ray position and direction
				double cameraX = 2 * x / (double)w - 1; //x-coordinate in camera space
				double rayDirX = dirX + planeX * cameraX;
				double rayDirY = dirY + planeY * cameraX;
				//which box of the map we're in
				int mapX = int(posX);
				int mapY = int(posY);

				//length of ray from current position to next x or y-side
				double sideDistX;
				double sideDistY;

				//length of ray from one x or y-side to next x or y-side
				double deltaDistX = std::abs(1 / rayDirX);
				double deltaDistY = std::abs(1 / rayDirY);
				double perpWallDist;

				//what direction to step in x or y-direction (either +1 or -1)
				int stepX;
				int stepY;

				int hit = 0; //was there a wall hit?
				int side; //was a NS or a EW wall hit?
				//calculate step and initial sideDist
				if (rayDirX < 0)
				{
					stepX = -1;
					sideDistX = (posX - mapX) * deltaDistX;
				}
				else
				{
					stepX = 1;
					sideDistX = (mapX + 1.0 - posX) * deltaDistX;
				}
				if (rayDirY < 0)
				{
					stepY = -1;
					sideDistY = (posY - mapY) * deltaDistY;
				}
				else
				{
					stepY = 1;
					sideDistY = (mapY + 1.0 - posY) * deltaDistY;
				}
				//perform DDA
				while (hit == 0)
				{
					//jump to next map square, OR in x-direction, OR in y-direction
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						mapX += stepX;
						side = 0;
					}
					else
					{
						sideDistY += deltaDistY;
						mapY += stepY;
						side = 1;
					}
					//Check if ray has hit a wall
					if (worldMap[mapX][mapY] > 0) hit = 1;
				}
				//Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
				if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
				else          perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

				//Calculate height of line to draw on screen
				int lineHeight = (int)(h / perpWallDist);

				//calculate lowest and highest pixel to fill in current stripe
				int drawStart = -lineHeight / 2 + h / 2;
				if (drawStart < 0)drawStart = 0;
				int drawEnd = lineHeight / 2 + h / 2;
				if (drawEnd >= h)drawEnd = h - 1;

				//choose wall color
				switch (worldMap[mapX][mapY])
				{
				case 1:
					if (side == 1) 
					{ 
						SDL_SetRenderDrawColor(renderer, 255 / 2, 255 / 2, 255 / 2, SDL_ALPHA_OPAQUE);
					}
					else
					{
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
					}

					break;
				default: 
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
					break;
				}

				//draw the pixels of the stripe as a vertical line
				SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
			}


			//Rendering
			SDL_RenderPresent(renderer);
		}
	}

	void cleanUp()
	{
		printf("Destroying instances and cleaning up.\n");

		//Destroy the renderer
		SDL_DestroyRenderer(renderer);

		//Destroy window
		SDL_DestroyWindow(window);

		//Quit SDL subsystems
		SDL_Quit();
	}
};
*/

class Test : public windowContext::App
{
public:
	void mainLoop() override
	{
		//timing for input and FPS counter
		oldTime = time;
		time = SDL_GetTicks();
		double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds

		//speed modifiers
		double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
		double rotSpeed = frameTime * 3.0; //the constant value is in radians/second

		//Map dimensions
		const int mapWidth = 10;
		const int mapHeight = 10;

		const int worldMap[mapWidth][mapHeight] = {
			{1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,1,0,0,0,1},
			{1,0,0,0,0,1,1,1,0,1},
			{1,0,0,0,0,0,0,1,0,1},
			{1,0,1,0,0,0,0,1,0,1},
			{1,0,1,0,0,0,0,1,0,1},
			{1,0,1,0,0,0,0,0,0,1},
			{1,0,1,1,1,0,0,0,0,1},
			{1,0,0,0,1,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1}
		};

		while (keepAlive) {
			SDL_Event event;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);

			//Event polling
			while (SDL_PollEvent(&event) != 0) {
				//User requests quit
				if (event.type == SDL_QUIT)
				{
					printf("Exiting\n");
					keepAlive = false;
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_SPACE:
						printf("Space\n");
						break;

					case SDLK_ESCAPE:
						printf("Exiting\n");
						keepAlive = false;
						break;

					case SDLK_w:
						printf("Pressed W\n");
						if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
						if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
						break;

					case SDLK_s:
						printf("Pressed S\n");
						if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
						if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
						break;

					case SDLK_RIGHT:
						printf("Pressed ->\n");
						oldDirX = dirX;
						dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
						dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
						oldPlaneX = planeX;
						planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
						planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
						break;

					case SDLK_LEFT:
						printf("Pressed <-\n");
						oldDirX = dirX;
						dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
						dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
						oldPlaneX = planeX;
						planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
						planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
						break;

					default:
						break;
					}
				}
			}
		
			for (int x = 0; x < getWidth(); x++)
			{
				//calculate ray position and direction
				double cameraX = 2 * x / (double)getWidth() - 1; //x-coordinate in camera space
				double rayDirX = dirX + planeX * cameraX;
				double rayDirY = dirY + planeY * cameraX;
				//which box of the map we're in
				int mapX = int(posX);
				int mapY = int(posY);

				//length of ray from current position to next x or y-side
				double sideDistX;
				double sideDistY;

				//length of ray from one x or y-side to next x or y-side
				double deltaDistX = std::abs(1 / rayDirX);
				double deltaDistY = std::abs(1 / rayDirY);
				double perpWallDist;

				//what direction to step in x or y-direction (either +1 or -1)
				int stepX;
				int stepY;

				int hit = 0; //was there a wall hit?
				int side; //was a NS or a EW wall hit?
				//calculate step and initial sideDist
				if (rayDirX < 0)
				{
					stepX = -1;
					sideDistX = (posX - mapX) * deltaDistX;
				}
				else
				{
					stepX = 1;
					sideDistX = (mapX + 1.0 - posX) * deltaDistX;
				}
				if (rayDirY < 0)
				{
					stepY = -1;
					sideDistY = (posY - mapY) * deltaDistY;
				}
				else
				{
					stepY = 1;
					sideDistY = (mapY + 1.0 - posY) * deltaDistY;
				}
				//perform DDA
				while (hit == 0)
				{
					//jump to next map square, OR in x-direction, OR in y-direction
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						mapX += stepX;
						side = 0;
					}
					else
					{
						sideDistY += deltaDistY;
						mapY += stepY;
						side = 1;
					}
					//Check if ray has hit a wall
					if (worldMap[mapX][mapY] > 0) hit = 1;
				}
				//Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
				if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
				else          perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

				//Calculate height of line to draw on screen
				int lineHeight = (int)(getHeight() / perpWallDist);

				//calculate lowest and highest pixel to fill in current stripe
				int drawStart = -lineHeight / 2 + getHeight() / 2;
				if (drawStart < 0)drawStart = 0;
				int drawEnd = lineHeight / 2 + getHeight() / 2;
				if (drawEnd >= getHeight())drawEnd = getHeight() - 1;

				//choose wall color
				switch (worldMap[mapX][mapY])
				{
				case 1:
					if (side == 1)
					{
						SDL_SetRenderDrawColor(renderer, 255 / 2, 255 / 2, 255 / 2, SDL_ALPHA_OPAQUE);
					}
					else
					{
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
					}

					break;
				default:
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
					break;
				}

				//draw the pixels of the stripe as a vertical line
				SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
			}


			//Rendering
			SDL_RenderPresent(renderer);

		}
	}
};


int main(int argc, char* args[])
{
	Test gameObj;

	try
	{
		gameObj.start(640, 480);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
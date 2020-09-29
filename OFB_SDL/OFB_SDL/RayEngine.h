#pragma once

#include <vector>

namespace rce
{
	class RCEngine
	{

	private:
		//Map dimensions
		static const int mapWidth = 20;
		static const int mapHeight = 20;
		int worldMap[mapWidth][mapHeight] = {
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

	public:
		double posX = 5, posY = 5;  //x and y start position
		double dirX = -1, dirY = 0; //initial direction vector
		double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
		double rotSpeed = frameTime * 3.0; //the constant value is in radians/second
		double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

		double oldDirX;
		double oldPlaneX;

		RCEngine();
		void moveRight();
		void moveLeft();
		void moveUp();
		void moveDown();
	};
	
};
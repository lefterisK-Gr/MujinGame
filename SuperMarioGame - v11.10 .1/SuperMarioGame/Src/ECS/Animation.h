#pragma once

struct Animation
{
	int indexX;
	int indexY;
	int frames;
	int speed;

	Animation() 
	{
		
	}
	Animation(int ix, int iy , int f, int s) // Animation frames look the next number of frames from the index
	{
		indexX = ix;
		indexY = iy;
		frames = f;
		speed = s;
	}

};
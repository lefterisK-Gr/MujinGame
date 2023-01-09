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
	Animation(int ix, int iy , int f, int s)
	{
		indexX = ix;
		indexY = iy;
		frames = f;
		speed = s;
	}

};
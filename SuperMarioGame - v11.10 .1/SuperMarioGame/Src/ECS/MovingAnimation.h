#pragma once

#include "Animation.h"

struct MovingAnimation : public Animation //todo moving animation can be moving sprite with of without transform
{
	int distanceX = 0;
	int distanceY = 0;

	MovingAnimation() : Animation()
	{

	}
	MovingAnimation(int ix, int iy, int f, float s, const std::string _type, int dx, int dy) : Animation(ix, iy, f, s, _type) // Animation frames look the next number of frames from the index
	{
		distanceX = dx;
		distanceY = dy;
	}

	MovingAnimation(int ix, int iy, int f, float s, const animType _type, int dx, int dy) : Animation(ix, iy, f, s, _type) // Animation frames look the next number of frames from the index
	{
		distanceX = dx;
		distanceY = dy;
	}

};
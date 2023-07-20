#pragma once

struct Animation
{
	typedef enum {
		ANIMTYPE_NONE = 0,
		ANIMTYPE_PLAY_ONCE = 1, // just iterates over the images one time. it holds the final image when finished.
		ANIMTYPE_LOOPED = 2, // going over the images again and again.
		ANIMTYPE_BACK_FORTH = 3  // iterate from index=0 to maxframe and back again. keeps holding the first image afterwards.
	} animType;

	int indexX;
	int indexY;
	int frames;
	float speed;

	animType type;

	Animation() 
	{
		
	}
	Animation(int ix, int iy , int f, int s, const std::string _type) // Animation frames look the next number of frames from the index
	{
		indexX = ix;
		indexY = iy;
		frames = f;
		speed = s;

		type = _type == "play_once" ? ANIMTYPE_PLAY_ONCE :
			_type == "back_forth" ? ANIMTYPE_BACK_FORTH :
			_type == "looped" ? ANIMTYPE_LOOPED :
			ANIMTYPE_NONE;
	}

};
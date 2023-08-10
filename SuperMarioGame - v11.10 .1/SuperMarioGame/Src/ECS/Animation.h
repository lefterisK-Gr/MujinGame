#pragma once

#include <string>

struct Animation //todo for now just add a bool hasFinished (useful for scripts) and much later may need onStop(when game paused)
{					// todo also we might need to have setReps/getReps and replace "play_once" with "play_n_times" where we pass 1 more value to call
	typedef enum {
		ANIMTYPE_NONE = 0,
		ANIMTYPE_PLAY_ONCE = 1, // just iterates over the images one time. it holds the final image when finished.
		ANIMTYPE_LOOPED = 2, // going over the images again and again.
		ANIMTYPE_BACK_FORTH = 3  // iterate from index=0 to maxframe and back again. keeps holding the first image afterwards.
	} animType;

	int indexX; // initial position
	int indexY;
	int total_frames;
	float speed;
	animType type;

	int cur_frame_index = 0;
	float cur_frame_index_f = 0;
	int times_played = 0;

	int flow_direction = 1;

	bool finished = false;

	Animation() 
	{
		
	}
	Animation(int ix, int iy , int f, float s, const std::string _type) // Animation frames look the next number of frames from the index
	{
		indexX = ix;
		indexY = iy;
		total_frames = f;
		speed = s;

		type = _type == "play_once" ? ANIMTYPE_PLAY_ONCE :
			_type == "back_forth" ? ANIMTYPE_BACK_FORTH :
			_type == "looped" ? ANIMTYPE_LOOPED :
			ANIMTYPE_NONE;
	}

	Animation(int ix, int iy, int f, float s, const animType _type) // Animation frames look the next number of frames from the index
	{
		indexX = ix;
		indexY = iy;
		total_frames = f;
		speed = s;

		type = _type;
	}

	void advanceFrame() {
		
		switch (type) {
		case Animation::animType::ANIMTYPE_PLAY_ONCE:
			cur_frame_index_f += speed;
			cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
			if (cur_frame_index > total_frames - 1) //essentially when we see that now we reach a frame out of total frames we reset it
			{
				resetFrameIndex();
				finished = true;
				times_played = 1;
			}

			break;

		case Animation::animType::ANIMTYPE_LOOPED:
			cur_frame_index_f += speed;
			cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
			if (cur_frame_index > total_frames - 1) {
				resetFrameIndex();
				times_played++;
			}
			break;

		case Animation::animType::ANIMTYPE_BACK_FORTH:

			if (flow_direction == 1) {
				cur_frame_index_f += speed;
				cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);

				if(cur_frame_index > total_frames - 1)
					flow_direction = -1;
			}
			else if (flow_direction == -1) {
				if (cur_frame_index > 0) {
					cur_frame_index_f -= speed;
					cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
				}
				else {
					flow_direction = 1;
					resetFrameIndex();
					finished = true;
					times_played = 1;
				}
			}
			break;

		case Animation::animType::ANIMTYPE_NONE:
			break;
		}
	}

	void resetFrameIndex() {
		cur_frame_index = 0;
		cur_frame_index_f = 0;
	}

	bool hasFinished() {
		return finished;
	}
};
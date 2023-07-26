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
	int total_frames;
	float speed;
	animType type;

	int cur_frame_index;
	float cur_frame_index_f;
	int times_played;

	

	Animation() 
	{
		
	}
	Animation(int ix, int iy , int f, int s, const std::string _type) // Animation frames look the next number of frames from the index
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

	Animation(int ix, int iy, int f, int s, const animType _type) // Animation frames look the next number of frames from the index
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

			if (cur_frame_index < total_frames) {
				cur_frame_index_f += speed;
				cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
			}
			else
				times_played = 1;
			break;

		case Animation::animType::ANIMTYPE_LOOPED:
			if (cur_frame_index < total_frames) {
				cur_frame_index_f += speed;
				cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
			}
			else {
				cur_frame_index = 0;
				cur_frame_index_f = 0;
				times_played++;
			}
			break;

	/*	case Animation::animType::ANIMTYPE_BACK_FORTH:

			if (additional_data == 1) {
				if (cur_frame_index < last_base_index) {
					cur_frame_index_f += speed;
					cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
				}
				else
					additional_data = -1;
			}
			else if (additional_data == -1) {
				if (cur_frame_index > 0) {
					cur_frame_index_f -= speed;
					cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);
				}
				else {
					additional_data = 1;
					times_played++;
				}
			}
			break;*/

		case Animation::animType::ANIMTYPE_NONE:
			break;
		}
	}

};
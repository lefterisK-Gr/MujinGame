#pragma once

#include "Animation.h"
#include "../Vertex.h"

struct FlashAnimation : public Animation //todo moving animation can be moving sprite with of without transform
{
	bool isFlashing = false;
	float flashDurationSpeed = 0.0f;  // Duration of flash effect in seconds
	float flashTimerSpeed = 0.0f; // Timer for flash effect
	Color flashColor;

	FlashAnimation() : Animation()
	{

	}
	// ix,iy is initial position (destX, destY), f is total frames to move, s is the speed to move frames, type as in animation, dx,dy distance to move
	FlashAnimation(int ix, int iy, int f, float s, const std::string _type, float flashD, float flashT, Color flashC, int _reps = 0) : Animation(ix, iy, f, s, _type) // Animation frames look the next number of frames from the index
	{
		flashDurationSpeed = flashD;
		flashTimerSpeed = flashT;
		flashColor = flashC;
		reps = _reps;
	}

	FlashAnimation(int ix, int iy, int f, float s, const animType _type, float flashD, float flashT, Color flashC, int _reps = 0) : Animation(ix, iy, f, s, _type) // Animation frames look the next number of frames from the index
	{
		flashDurationSpeed = flashD;
		flashTimerSpeed = flashT;
		flashColor = flashC;
		reps = _reps;
	}

	void advanceFrame(float deltaTime) {
		unsigned short prev_frame_index = cur_frame_index;

		speed = isFlashing ? flashDurationSpeed : flashTimerSpeed;
		switch (type) {
		case Animation::animType::ANIMTYPE_LOOPED:
		case Animation::animType::ANIMTYPE_PLAY_N_TIMES:
			cur_frame_index_f += speed * deltaTime;
			cur_frame_index = static_cast<unsigned short>(cur_frame_index_f);

			// Check if the frame index has changed
			if (prev_frame_index != cur_frame_index) {
				frame_times_played = 1;
				isFlashing = !isFlashing;
			}
			else {
				frame_times_played++;
			}

			if (cur_frame_index > total_frames - 1) //essentially when we see that now we reach a frame out of total frames we reset it
			{
				resetFrameIndex();
				times_played++;
				if (reps && times_played >= reps) {
					finished = true;
				}
			}
			break;

		case Animation::animType::ANIMTYPE_NONE:
			break;
		}
	}

};
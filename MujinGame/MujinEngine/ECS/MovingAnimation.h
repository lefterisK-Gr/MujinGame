#pragma once

#include "Animation.h"

class MovingAnimation : public Animation //todo moving animation can be moving sprite with of without transform
{
public:
	std::vector<glm::vec2> positions;  // Stores the positions for the animation
	std::vector<int> zIndices;         // zIndex for each position
	std::vector<int> rotations;

	int distanceX = 0;
	int distanceY = 0;

	MovingAnimation() : Animation()
	{

	}
	// ix,iy is initial position (destX, destY), f is total frames to move, s is the speed to move frames, type as in animation, dx,dy distance to move
	MovingAnimation(int ix, int iy, int f, float s, const std::string _type, int dx, int dy, int _reps = 0) : Animation(ix, iy, f, s, _type) // Animation frames look the next number of frames from the index
	{
		distanceX = dx;
		distanceY = dy;
		reps = _reps;
	}

	MovingAnimation(int ix, int iy, int f, float s, const animType _type, int dx, int dy, int _reps = 0) : Animation(ix, iy, f, s, _type) // Animation frames look the next number of frames from the index
	{
		distanceX = dx;
		distanceY = dy;
		reps = _reps;
	}

	MovingAnimation(const std::vector<glm::vec2>& _positions, const std::vector<int>& _zIndices, const std::vector<int>& _rotations,
		int ix, int iy, int f, float s, const std::string _type, int _reps = 0) : Animation(ix, iy, f, s, _type), positions(_positions),
		zIndices(_zIndices), rotations(_rotations) // Animation frames look the next number of frames from the index
	{
		reps = _reps;
		validateVectors();
	}

	MovingAnimation(const std::vector<glm::vec2>& _positions, const std::vector<int>& _zIndices, const std::vector<int>& _rotations, 
		int ix, int iy, int f, float s, const animType _type, int _reps = 0) : Animation(ix, iy, f, s, _type), positions(_positions), 
		zIndices(_zIndices), rotations(_rotations) // Animation frames look the next number of frames from the index
	{
		reps = _reps;
		validateVectors();
	}
private:
	void validateVectors() const {
		// Ensure all vectors are of the same length to prevent indexing errors
		if (positions.size() != zIndices.size() || positions.size() != rotations.size()) {
			throw std::invalid_argument("All vectors (positions, zIndices, rotations) must have the same length.");
		}
	}
};
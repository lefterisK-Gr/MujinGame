#pragma once

#include "Components.h"

class ScoreComponent : public Component
{
public:
	int score;
	ScoreComponent()
	{
		score = 0;
	}
	ScoreComponent(int scr)
	{
		score = scr;
	}
	~ScoreComponent()
	{}

	void init() override
	{

	}

	void update() override
	{

	}

	void addToScore(int scr)
	{
		score += scr;
	}

	int getScore()
	{
		return score;
	}
};
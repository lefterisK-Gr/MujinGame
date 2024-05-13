#pragma once

#include "ECS/Components.h"

class ScoreComponent : public Component
{
private:
	int score;
	Entity* scorelabel;
public:
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
		scorelabel = &manager.addEntity();
		scorelabel->addComponent<TransformComponent>(32, 576, 32, 32, 1);
		scorelabel->addComponent<UILabel>("score 0", "arial", true);
		scorelabel->addGroup(Manager::groupLabels);
	}

	void update(float deltaTime) override
	{

	}

	void addToScore(int scr)
	{
		score += scr;
		scorelabel->GetComponent<UILabel>().setLetters("score" + std::to_string(score));
	}

	int getScore()
	{
		return score;
	}
};
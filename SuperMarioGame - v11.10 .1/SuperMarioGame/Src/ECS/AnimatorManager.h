#pragma once

#include <map>
#include "Animation.h"
#include "MovingAnimation.h"


struct AnimatorManager
{
public:
	std::map<const char*, Animation> animations;
	std::map<const char*, MovingAnimation> moving_animations;

	AnimatorManager()
	{
		Animation defaultAnimation = Animation(6, 2, 1, 0.04, "looped");

		Animation p1idle = Animation(0, 0, 10, 0.1, "looped");
		Animation p1walk = Animation(0, 2, 10, 0.1, "looped");
		Animation p1jump = Animation(7, 6, 1, 0.1, "looped");
		Animation p1attack = Animation(0, 3, 10, 0.4, "play_n_times", 1);

		Animation question_mark = Animation(1, 9, 3, 0.02, "looped");
		Animation brown_block = Animation(3, 0, 1, 0.005, "looped");
		Animation dark_block = Animation(2, 12, 1, 0.005, "looped");
		Animation coin_flip = Animation(0, 1, 3, 0.02, "looped");

		Animation goomba_walk = Animation(17, 1, 2, 0.04, "looped");
		Animation greenkoopatroopa_walk = Animation(20, 1, 2, 0.04, "looped");
		Animation greenshell = Animation(25, 1, 1, 0.01, "play_n_times", 1);

		animations.emplace("Default", defaultAnimation);
		animations.emplace("P1Idle", p1idle);
		animations.emplace("P1Walk", p1walk);
		animations.emplace("P1Jump", p1jump);
		animations.emplace("P1Attack", p1attack);

		animations.emplace("QuestionMark", question_mark);
		animations.emplace("BrownBlock", brown_block);
		animations.emplace("DarkBlock", dark_block);
		animations.emplace("CoinFlip", coin_flip);
		animations.emplace("GoombaWalk", goomba_walk);
		animations.emplace("GreenKoopaTroopaWalk", greenkoopatroopa_walk);
		animations.emplace("GreenShell", greenshell);

		MovingAnimation defaultMoveAnimation = MovingAnimation(0, 0, 0, 0.0, "looped", 0, 0);
		MovingAnimation playerVertTransition = MovingAnimation(0, 0, 20, 1, "play_n_times", 0, 1, 1);
		MovingAnimation playerHorTransition = MovingAnimation(0, 0, 20, 1, "play_n_times", 1, 0, 1);

		MovingAnimation block_bounce = MovingAnimation(0, 0, 10, 0.5, "back_forth", 0, -4, 1);
		MovingAnimation coin_bounce = MovingAnimation(0, 0, 20, 1, "back_forth", 0, -2, 1);
		
		moving_animations.emplace("Default", defaultMoveAnimation);
		moving_animations.emplace("PlayerVertTransition", playerVertTransition);
		moving_animations.emplace("PlayerHorTransition", playerHorTransition);
		moving_animations.emplace("BlockBounce", block_bounce);
		moving_animations.emplace("CoinBounce", coin_bounce);
	}
};

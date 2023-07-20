#pragma once

#include <map>
#include "Animation.h"


struct AnimatorManager
{
public:
	std::map<const char*, Animation> animations;

	AnimatorManager()
	{
		Animation p1idle = Animation(6, 2, 1, 100, "looped");
		Animation p1walk = Animation(0, 2, 3, 100, "looped");
		Animation p1jump = Animation(4, 2, 1, 100, "looped");
		Animation p2idle = Animation(6, 8, 1, 100, "looped");
		Animation p2walk = Animation(0, 8, 3, 100, "looped");
		Animation p2jump = Animation(4, 8, 1, 100, "looped");
		Animation question_mark = Animation(0, 0, 3, 200, "looped");
		Animation brown_block = Animation(3, 0, 1, 50, "looped");
		Animation dark_block = Animation(2, 12, 1, 50, "looped");
		Animation coin_flip = Animation(0, 1, 3, 200, "looped");
		Animation goomba_walk = Animation(17, 1, 2, 400, "looped");
		Animation greenkoopatroopa_walk = Animation(20, 1, 2, 400, "looped");
		Animation greenshell = Animation(25, 1, 1, 10000, "looped");

		animations.emplace("P1Idle", p1idle);
		animations.emplace("P1Walk", p1walk);
		animations.emplace("P1Jump", p1jump);
		animations.emplace("P2Idle", p2idle);
		animations.emplace("P2Walk", p2walk);
		animations.emplace("P2Jump", p2jump);
		animations.emplace("QuestionMark", question_mark);
		animations.emplace("BrownBlock", brown_block);
		animations.emplace("DarkBlock", dark_block);
		animations.emplace("CoinFlip", coin_flip);
		animations.emplace("GoombaWalk", goomba_walk);
		animations.emplace("GreenKoopaTroopaWalk", greenkoopatroopa_walk);
		animations.emplace("GreenShell", greenshell);
	}
};

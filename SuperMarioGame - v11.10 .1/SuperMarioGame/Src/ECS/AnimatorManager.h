#pragma once

#include <map>
#include "Animation.h"
#include "MovingAnimation.h"
#include "FlashAnimation.h"


struct AnimatorManager
{
public:
	std::map<const char*, Animation> animations;
	std::map<const char*, MovingAnimation> moving_animations;
	std::map<const char*, FlashAnimation> flash_animations;

	AnimatorManager()
	{
		Animation defaultAnimation = Animation(6, 2, 1, 0.04, "looped");

		Animation p1idle = Animation(0, 0, 4, 0.1, "looped");
		Animation p1walk = Animation(0, 3, 8, 0.1, "looped");
		Animation p1jump = Animation(0, 2, 2, 0.1, "looped");
		Animation p1attack = Animation(0, 1, 4, 0.1, "play_n_times", 1);
		Animation p1ability1 = Animation(0, 1, 4, 0.04, "play_n_times", 1);

		Animation question_mark = Animation(1, 9, 3, 0.05, "back_forth");
		Animation brown_block = Animation(3, 0, 1, 0.005, "looped");
		Animation dark_block = Animation(2, 12, 1, 0.005, "looped");
		Animation coin_flip = Animation(1, 7, 2, 0.2, "looped");

		Animation skeleton_idle = Animation(0, 0, 4, 0.04, "looped");
		Animation skeleton_walk = Animation(0, 1, 4, 0.04, "looped");
		Animation skeleton_attack = Animation(0, 2, 8, 0.05, "play_n_times", 1);
		Animation greenkoopatroopa_idle = Animation(0, 0, 4, 0.1, "looped");
		Animation greenkoopatroopa_walk = Animation(0, 2, 10, 0.1, "looped");
		Animation greenkoopatroopa_attack = Animation(0, 1, 8, 0.1, "play_n_times", 1);
		Animation greenshell = Animation(0, 0, 4, 0.1, "play_n_times", 5);

		animations.emplace("Default", defaultAnimation);
		animations.emplace("P1Idle", p1idle);
		animations.emplace("P1Walk", p1walk);
		animations.emplace("P1Jump", p1jump);
		animations.emplace("P1Attack", p1attack);
		animations.emplace("P1Ability1", p1ability1);

		animations.emplace("QuestionMark", question_mark);
		animations.emplace("BrownBlock", brown_block);
		animations.emplace("DarkBlock", dark_block);
		animations.emplace("CoinFlip", coin_flip);
		animations.emplace("SkeletonIdle", skeleton_idle);
		animations.emplace("SkeletonWalk", skeleton_walk);
		animations.emplace("SkeletonAttack", skeleton_attack);
		animations.emplace("GreenKoopaTroopaIdle", greenkoopatroopa_idle);
		animations.emplace("GreenKoopaTroopaWalk", greenkoopatroopa_walk);
		animations.emplace("GreenKoopaTroopaAttack", greenkoopatroopa_attack);
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

		FlashAnimation defaultFlashAnimation = FlashAnimation(0, 0, 1, 0, "looped", 0, 0, {255,255,255,255});
		FlashAnimation playerHit = FlashAnimation(0, 0, 1, 0, "play_n_times", 0.2, 0.2, { 255,255,255,50 }, 10);
		flash_animations.emplace("Default", defaultFlashAnimation);
		flash_animations.emplace("PlayerHit", playerHit);
	}
};

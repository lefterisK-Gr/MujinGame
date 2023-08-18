#pragma once


#include "Components.h"
#include <map>
#include "Animation.h"
#include "AnimatorManager.h"
// TODO: in comparison to AnimatorComponent, here we also have access to sprite so create functions like SetDx,SetDy,SetRepsOfMove,SetDelayOfMove, IsForeverRepeatedMove

class MovingAnimatorComponent : public AnimatorComponent //Animator -> Sprite -> Transform 
{					//! also we use MovingAnimator instead of simple Animator so that entities use less memory and we use it to entities that have triggers that change their animation
public:
	//std::map<const char*, Animation> animations; //Animator Manager

	MovingAnimatorComponent() : AnimatorComponent()
	{

	}

	MovingAnimatorComponent(std::string id) : AnimatorComponent(id)
	{

	}

	~MovingAnimatorComponent()
	{

	}

	void init() override
	{
		if (!entity->hasComponent<SpriteComponent>())
		{
			entity->addComponent<SpriteComponent>(textureid);
		}
		sprite = &entity->getComponent<SpriteComponent>();

		Play("Default");
		sprite->setTex(textureid);
	}

	void update() override
	{
		if (animimationName == "BlockBounce")
		{
			std::cout << animimationName << std::endl;
		}
		if (sprite->moving_animation.hasFinished()) { // playing again animation
			sprite->moving_animation.finished = false;
			sprite->moving_animation.times_played = 0;
			resetAnimation();
		}

		sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;

		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale;

		sprite->moving_animation.advanceFrame();
		sprite->setMoveFrame();
	}

	void draw() override
	{
		//sprite->draw();
	}

	void Play(const char* animName)
	{
		animimationName = animName;
		sprite->SetMovingAnimation(
			animManager.moving_animations[animimationName].indexX, animManager.moving_animations[animimationName].indexY,
			animManager.moving_animations[animimationName].total_frames, animManager.moving_animations[animimationName].speed,
			animManager.moving_animations[animimationName].type,
			animManager.moving_animations[animimationName].distanceX, animManager.moving_animations[animimationName].distanceY
		);
	}

	void resetAnimation() {
		animimationName = "Default";
		sprite->SetMovingAnimation(
			animManager.moving_animations[animimationName].indexX, animManager.moving_animations[animimationName].indexY,
			animManager.moving_animations[animimationName].total_frames, animManager.moving_animations[animimationName].speed,
			animManager.moving_animations[animimationName].type,
			animManager.moving_animations[animimationName].distanceX, animManager.moving_animations[animimationName].distanceY);
	}

	const char* getPlayName()
	{
		return animimationName;
	}

	void DestroyTex()
	{
		sprite->DestroyTex();
	}

};
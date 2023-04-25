#pragma once

#include "Components.h"
#include <map>
#include "Animation.h"
#include "AnimatorManager.h"

class AnimatorComponent : public Component //Animator -> Sprite -> Transform
{
public:
	SpriteComponent* sprite;
	AnimatorManager animManager;
	std::string textureid;
	const char* animimationName = NULL;
	uint32_t resumeTime = 0; //this may need to be in animatorManager

	//std::map<const char*, Animation> animations; //Animator Manager

	AnimatorComponent()
	{

	}

	AnimatorComponent(std::string id)
	{
		textureid = id;
	}

	~AnimatorComponent()
	{

	}

	void init() override
	{
		if (!entity->hasComponent<SpriteComponent>())
		{
			entity->addComponent<SpriteComponent>(textureid);
		}
		sprite = &entity->getComponent<SpriteComponent>();

		Play("P1Idle");
		sprite->setTex(textureid);
	}

	void update() override
	{
		int timeslice = 0;

		sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;

		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale;
	}

	void draw() override
	{
		//sprite->draw();
	}

	void Play(const char* animName)
	{
		animimationName = animName;
		sprite->SetFrames(animManager.animations[animName].indexX, animManager.animations[animName].indexY, animManager.animations[animName].frames, animManager.animations[animName].speed);
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
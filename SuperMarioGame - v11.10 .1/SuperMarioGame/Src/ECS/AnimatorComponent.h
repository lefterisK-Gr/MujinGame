#pragma once

#include "Components.h"
#include <map>
#include "Animation.h"
#include "AnimatorManager.h"
#include <functional>
// TODO: add OnFinish, OnStart (+OnAction, where it is updated based on conditions, like attacking while on jump (different attack than normal)) functions
typedef uint32_t timestamp;


class AnimatorComponent : public Component //Animator -> Sprite -> Transform
{
public:

	// onAction is the same thing as Play()
	// onFinish and onStart are for when we free Animator and when we initialize it
	// difference between this and from the lectures is that in lectures it uses seperate animator for each animation

	SpriteComponent* sprite;
	AnimatorManager animManager;
	std::string textureid;
	const char* animimationName = NULL;
	timestamp resumeTime = 0;

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

		Play("P1Idle"); //onStart
		sprite->setTex(textureid);
	}

	void update() override //onAction
	{
		sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;

		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale;

		sprite->animation.advanceFrame();
		sprite->setCurrFrame();
	}

	void draw() override
	{
		//sprite->draw();
	}

	void Play(const char* animName)
	{
		animimationName = animName;
		sprite->SetAnimation(animManager.animations[animName].indexX, animManager.animations[animName].indexY, animManager.animations[animName].total_frames, animManager.animations[animName].speed, animManager.animations[animName].type );
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
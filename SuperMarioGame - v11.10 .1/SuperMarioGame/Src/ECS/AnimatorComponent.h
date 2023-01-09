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
	bool animated = false;
	bool didCoinAnimation = false;
	bool blockAnimation = false;
	bool didBlockAnimation = false;
	bool vertTransitionPlayerAnimation = false;
	bool horTransitionPlayerAnimation = false;
	bool finishedVertAnimation = false;
	bool finishedHorAnimation = false;
	bool shelltoturtle = false;
	//std::map<const char*, Animation> animations;

	AnimatorComponent()
	{
		
	}

	AnimatorComponent(std::string id, bool isAnimated, bool isBlockAnim)
	{
		textureid = id;
		animated = isAnimated;
		blockAnimation = isBlockAnim;
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
		
		if (animated)
		{
			sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / sprite->speed) % sprite->frames));
			if (textureid == "greenkoopatroopa")
			{
				sprite->srcRect.y = sprite->animIndexY * sprite->transform->height - 20;
				sprite->srcRect.h = sprite->transform->height + 20;
			}
			else
			{
				sprite->srcRect.y = sprite->animIndexY * sprite->transform->height;
			}
		}

		if (didCoinAnimation) //add finished coin animation so its not checked everytime
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}	
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 3));
			sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) - (20 * timeslice) - Game::camera.y);
			if (timeslice == 3)
			{
				sprite->destRect.y += 40;
			}
			else if (timeslice == 4)
			{
				sprite->destRect.y += 80;
			}
			else if (timeslice == 5) // on finish
			{
				sprite->DestroyTex();
			}
		}
		else if (vertTransitionPlayerAnimation)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - resumeTime -sprite->initTime) / sprite->speed) % (sprite->frames + 16));
			sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) + (2 * timeslice) - Game::camera.y);
			if (timeslice == 16)// on finish
			{
				vertTransitionPlayerAnimation = false;
				sprite->initTime = 0;
				finishedVertAnimation = true;
			}
		}
		else if (horTransitionPlayerAnimation)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 16));
			sprite->destRect.x = (static_cast<int>(sprite->transform->position.x) + (2 * timeslice) - Game::camera.x);
			if (timeslice == 16)// on finish
			{
				horTransitionPlayerAnimation = false;
				sprite->initTime = 0;
				finishedHorAnimation = true;
			}
		}
		else if (blockAnimation && didBlockAnimation)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 4));
			sprite->destRect.y = (static_cast<int>(sprite->transform->position.y) - (8 * (timeslice+1)) - Game::camera.y);
			
			if (timeslice == 2)
			{
				sprite->destRect.y += 16;
			}
			else if (timeslice == 3)// on finish
			{
				sprite->destRect.y += 32;
				sprite->initTime = 0;
				didBlockAnimation = false;
			}
		}
		else if (shelltoturtle)
		{
			if (!sprite->initTime)
			{
				sprite->initTime = SDL_GetTicks();
			}
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			sprite->destRect.y = static_cast<int>(sprite->transform->position.y - 20) - Game::camera.y; //make player move with the camera, being stable in centre, except on edges
			if (Game::justResumed)//if we just resumed
			{
				resumeTime += SDL_GetTicks() - Game::pauseTime;
			}
			timeslice = static_cast<int>(((SDL_GetTicks() - resumeTime - sprite->initTime) / sprite->speed) % (sprite->frames + 2));
			if (timeslice == 1)// on finish
			{
				sprite->initTime = 0;
				shelltoturtle = false;
				Play("GreenKoopaTroopaWalk");
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->transform->velocity.x = -1;
			}
		}
		else
		{
			sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
			if (textureid == "greenkoopatroopa")
			{
				sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y - 20;
			}
			else
			{
				sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;
			}
		}
		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		if (textureid == "greenkoopatroopa")
		{
			sprite->destRect.h = sprite->transform->height * sprite->transform->scale + 20;
		}
		else 
		{
			sprite->destRect.h = sprite->transform->height * sprite->transform->scale;
		}
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
		didCoinAnimation = true;
	}

	bool getCoinAnimation()
	{
		return didCoinAnimation;
	}
};
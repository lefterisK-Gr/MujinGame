#pragma once

#include "Components.h"
#include <map>
#include "Animation.h"
#include "AnimatorManager.h"
#include <functional>
// TODO: add OnFinish, OnStart (+OnAction, where it is updated based on conditions, like attacking while on jump (different attack than normal)) functions
typedef uint32_t timestamp;

enum animatorstate {
	ANIMATOR_FINISHED = 0, ANIMATOR_RUNNING = 1, ANIMATOR_STOPPED = 2
};


class AnimatorComponent : public Component //Animator -> Sprite -> Transform
{
public:

	using OnFinish = std::function<void(AnimatorComponent*)>;
	using OnStart = std::function<void(AnimatorComponent*)>;
	using OnAction = std::function<void(AnimatorComponent*, const char* animName)>;

	OnFinish onFinish;
	OnStart onStart;
	OnAction onAction;

	SpriteComponent* sprite;
	AnimatorManager animManager;
	std::string textureid;
	const char* animimationName = NULL;
	timestamp resumeTime = 0; //this may need to be in animatorManager

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
		sprite->srcRect.x = (sprite->animIndexX * sprite->transform->width) /* init */ + (sprite->srcRect.w * static_cast<int>((SDL_GetTicks() / (int)sprite->speed) % sprite->total_frames)/* curframe from total frams */);
		sprite->srcRect.y = sprite->animIndexY * sprite->transform->height;

		sprite->destRect.x = static_cast<int>(sprite->transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		sprite->destRect.y = static_cast<int>(sprite->transform->position.y) - Game::camera.y;

		sprite->destRect.w = sprite->transform->width * sprite->transform->scale;
		sprite->destRect.h = sprite->transform->height * sprite->transform->scale;

		sprite->animation.advanceFrame();
		sprite->setFrame();
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

	void NotifyStarted(void) {
		if (onStart)
			(onStart)(this);
	}

	void NotifyStopped(void) {
		//state = ANIMATOR_STOPPED;
		if (onFinish)
			(onFinish)(this);
	}

	void NotifyAction(const char* animName) {
		if (onAction) {
			onAction(this, animName);
		}
	}

	template <typename Tfunc> void SetOnFinish(const Tfunc& f) { onFinish = f; }
	template <typename Tfunc> void SetOnStart(const Tfunc& f) { onStart = f; }
	template <typename Tfunc> void SetOnAction(const Tfunc& f) { onAction = f; }

};
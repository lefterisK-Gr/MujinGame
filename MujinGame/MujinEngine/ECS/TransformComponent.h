#pragma once

#include "Components.h"


class TransformComponent : public Component //transform as in graphics, we have rotation and scale
{
private:
	float _zIndex = 0.0f;
	float parallaxFactor = 1.0f;

	glm::vec2 _position;
	glm::vec2 _velocity;
public:
	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 1;

	bool activatedMovement = false;

	TransformComponent()
	{
		_position = glm::zero<glm::vec2>();
	}

	TransformComponent(int sc)
	{
		_position = glm::zero<glm::vec2>();
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		_position.x = x;
		_position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	TransformComponent(float x, float y, int h, int w, int sc, int sp)
	{
		_position.x = x;
		_position.y = y;
		height = h;
		width = w;
		scale = sc;
		speed = sp;
	}
	
	void init() override
	{
		parallaxFactor = 1.0f / _zIndex;
		_velocity = glm::zero<glm::ivec2>();
	}
	void update(float deltaTime) override
	{

		std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

		SDL_Rect dimensions = { _position.x, _position.y, width, height };
		SDL_Rect cameraDimensions = main_camera2D->getCameraRect();

		if (entity->checkCollision(dimensions, cameraDimensions)) { //culling
			entity->paused = false;
		}
		else {
			entity->paused = true;
			if (!entity->hasGroup(static_cast<Group>(17)) &&
				!entity->hasGroup(static_cast<Group>(0)) &&
				!entity->hasGroup(static_cast<Group>(11)) &&
				!entity->hasGroup(static_cast<Group>(12)) &&
				!entity->hasGroup(static_cast<Group>(13)) &&
				!entity->hasGroup(static_cast<Group>(4))) {
				return;
			}
		}
			
		_position.x += _velocity.x * speed * deltaTime;
		_position.y += _velocity.y * speed; //needs to have deltaTime
	}

	glm::vec2 getCenterTransform()
	{
		return glm::vec2(_position.x + width * scale / 2, _position.y + height * scale / 2);
	}

	float getZIndex() {
		return _zIndex;
	}
	void setZIndex( float newZIndex) {
		_zIndex = newZIndex;
	}

	glm::ivec2 getPosition() {
		return _position;
	}

	void setPosition_X(int newPosition_X) {
		_position.x = newPosition_X;
	}
	void setPosition_Y(int newPosition_Y) {
		_position.y = newPosition_Y;
	}

	glm::ivec2 getVelocity() {
		return _velocity;
	}

	void setVelocity_X(int newVelocity_X) {
		_velocity.x = newVelocity_X;
	}
	void setVelocity_Y(int newVelocity_Y) {
		_velocity.y = newVelocity_Y;
	}
};
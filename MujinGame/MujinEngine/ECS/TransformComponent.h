#pragma once

#include "Components.h"


class TransformComponent : public Component //transform as in graphics, we have rotation and scale
{
private:
	float _zIndex = -5.0f;

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

	TransformComponent(glm::vec2 position)
	{
		_position = position;
	}

	TransformComponent(glm::vec2 position, glm::ivec2 size, int sc) : TransformComponent(position){
		width = size.x;
		height = size.y;
		scale = sc;
	}

	TransformComponent(glm::vec2 position, glm::ivec2 size, int sc, int sp) : TransformComponent(position, size, sc)
	{
		speed = sp;
	}

	
	
	void init() override
	{
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
		_position.y += _velocity.y * speed ; //needs to have deltaTime
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

	glm::vec2 getPosition() {
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
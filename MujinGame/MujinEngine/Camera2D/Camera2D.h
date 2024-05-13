#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>
#include "ICamera.h"

class Camera2D : public ICamera{
public:
	SDL_Rect worldLocation;

	Camera2D() : _position(0.0f, 0.0f),
		_cameraMatrix(1.0f),	//I
		_orthoMatrix(1.0f),		//I
		_scale(1.0f),
		_cameraChange(true),
		_screenWidth(800),
		_screenHeight(640)
	{

	}
	~Camera2D()
	{

	}

	void init(int screenWidth, int screenHeight) override {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, (float)_screenHeight, 0.0f ); //left, right, top, bottom
	}

	void update() override {

		if (_cameraChange) {
			glm::vec3 translate(-_position.x, -_position.y, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate); //if glm ortho = -1,1,-1,1 then 1 horizontal with -400,-320 to bottom-left
			glm::vec3 scale(_scale, _scale, 0.0f);

			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			//_cameraMatrix = glm::scale(_cameraMatrix, scale);
			_cameraChange = false;
		}
	}

	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const override {
		//Make 0 the center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//Scale coordinates
		screenCoords /= _scale;
		//Translate with the camera2D.worldLocation position
		screenCoords += _position;
		return screenCoords;
	}

	//setters
	void setPosition(const glm::vec2& newPosition) override {
		_position = newPosition;
		_cameraChange = true;
	}

	void setScale(float newScale) override {
		_scale = newScale;
		_cameraChange = true;
	}

	//getters
	glm::vec2 getPosition() const override {
		return _position;
	}

	float getScale() const override {
		return _scale;
	}

	glm::mat4 getCameraMatrix() const override {
		return _cameraMatrix;
	}

	glm::vec2 getCameraDimensions() const override {
		glm::vec2 cameraDimensions = { _screenWidth, _screenHeight };
		return cameraDimensions;
	}

	SDL_Rect getCameraRect() const override {
		int cameraWidth = getCameraDimensions().x / getScale();
		int cameraHeight = getCameraDimensions().y / getScale();

		int cameraX = getCameraDimensions().x / 2 - cameraWidth / 2;
		int cameraY = getCameraDimensions().y / 2 - cameraHeight / 2;

		SDL_Rect cameraRect = { cameraX, cameraY, cameraWidth, cameraHeight };
		return cameraRect;
	}

private:
	int _screenWidth, _screenHeight;
	bool _cameraChange;
	float _scale;
	glm::vec2 _position;
	glm::mat4 _cameraMatrix;
	glm::mat4 _orthoMatrix;
};
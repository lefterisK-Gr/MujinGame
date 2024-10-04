#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>
#include "ICamera.h"

class Camera2D : public ICamera{
public:
	glm::ivec2 worldDimensions;

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

		GLdouble vvLeft = 0.0f;
		GLdouble vvRight = (float)_screenWidth;
		GLdouble vvBottom = (float)_screenHeight;
		GLdouble vvTop = 0.0f;
		GLdouble vvNear = 0.1f;
		GLdouble vvFar = 200.0f;

		GLdouble vvDepth = vvFar - vvNear;
		GLdouble vvHeight = vvTop - vvBottom;

		const GLdouble vvFovDegs = 45.0;
		GLdouble vvFovRads = 1.0f;

		_orthoMatrix[0][0] = (2) / (vvRight - vvLeft);
		_orthoMatrix[1][1] = (2) / (vvTop - vvBottom);
		_orthoMatrix[3][0] = -(vvRight + vvLeft) / (vvRight - vvLeft);
		_orthoMatrix[3][1] = -(vvTop + vvBottom) / (vvTop - vvBottom);
		_orthoMatrix[3][2] = -(vvFar + vvNear) / (vvFar - vvNear);

		_orthoMatrix[2][1] = (vvTop + vvBottom) / (vvTop - vvBottom);
		_orthoMatrix[2][2] = -(vvFar + vvNear) / (vvFar - vvNear);
		_orthoMatrix[2][3] = (-1);
		/*Result[3][2] = -(static_cast<T>(2) * farVal * nearVal) / (farVal - nearVal); */
		//glm::frustum
		//_orthoMatrix = glm::frustum(vvLeft, vvRight, vvBottom, vvTop, vvNear, vvFar); //left, right, top, bottom
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
		screenCoords += glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//Translate with the camera2D.worldLocation position
		screenCoords.x += _position.x;
		screenCoords.y += _position.y;


		return screenCoords;
	}

	//setters
	void setPosition_X(const float newPosition) override {
		_position.x = newPosition;
		_cameraChange = true;
	}

	void setPosition_Y(const float newPosition) override {
		_position.y = newPosition;
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

	glm::ivec2 getCameraDimensions() const override {
		glm::vec2 cameraDimensions = { _screenWidth, _screenHeight };
		return cameraDimensions;
	}

	SDL_Rect getCameraRect() const override {
		float cameraWidth = getCameraDimensions().x / getScale();
		float cameraHeight = getCameraDimensions().y / getScale();

		int cameraX = _position.x - cameraWidth / 2.0f + getCameraDimensions().x / 2;
		int cameraY = _position.y - cameraHeight / 2.0f + getCameraDimensions().y / 2;

		SDL_Rect cameraRect = { cameraX , cameraY , cameraWidth, cameraHeight };
		return cameraRect;
	}

private:
	int _screenWidth, _screenHeight;
	bool _cameraChange;
	float _scale;
	glm::vec2 _position;
	glm::mat4 _cameraMatrix;
	glm::mat4 _orthoMatrix; // changed once in init
};
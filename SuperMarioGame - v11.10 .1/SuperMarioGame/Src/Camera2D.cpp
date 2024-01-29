#include "Camera2D.h"

Camera2D::Camera2D() : _position(0.0f, 0.0f),
	_cameraMatrix(1.0f),	//I
	_orthoMatrix(1.0f),		//I
	_scale(1.0f),
	_cameraChange(true),
	_screenWidth(960),
	_screenHeight(768)
{

}

Camera2D::~Camera2D()
{

}

void Camera2D::init(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, -(float)_screenHeight); //left, right, bottom, top
}

void Camera2D::update() {

	if (_cameraChange) {
		glm::vec3 translate(-_position.x, -_position.y, 0.0f);
		_cameraMatrix = glm::translate(_orthoMatrix, translate); //if glm ortho = -1,1,-1,1 then 1 horizontal with -400,-320 to bottom-left
		glm::vec3 scale(_scale, _scale, 0.0f);

		_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

		//_cameraMatrix = glm::scale(_cameraMatrix, scale);
		_cameraChange = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
	//Make 0 the center
	screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
	//Scale coordinates
	screenCoords /= _scale;
	//Translate with the camera position
	screenCoords += _position;
	return screenCoords;
}

glm::vec2 Camera2D::getCameraDimensions(){
	glm::vec2 cameraDimensions = { _screenWidth, _screenHeight };
	return cameraDimensions;
}

SDL_Rect Camera2D::getCameraRect() {
	int cameraWidth = getCameraDimensions().x / getScale();
	int cameraHeight = getCameraDimensions().y / getScale();

	int cameraX = getCameraDimensions().x / 2 - cameraWidth / 2;
	int cameraY = getCameraDimensions().y / 2 - cameraHeight / 2;

	SDL_Rect cameraRect = { cameraX, cameraY, cameraWidth, cameraHeight };
	return cameraRect;
}
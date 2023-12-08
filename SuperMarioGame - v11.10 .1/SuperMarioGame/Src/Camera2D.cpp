#include "Camera2D.h"

Camera2D::Camera2D() : _position(0.0f, 0.0f),
	_cameraMatrix(1.0f),	//I
	_orthoMatrix(1.0f),		//I
	_scale(1.0f),
	_cameraChange(true),
	_screenWidth(800),
	_screenHeight(640)
{

}

Camera2D::~Camera2D()
{

}

void Camera2D::init(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight); //left, right, bottom, top
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
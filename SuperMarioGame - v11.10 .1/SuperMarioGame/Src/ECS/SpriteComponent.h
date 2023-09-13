#pragma once

#include "Components.h"
#include "SDL.h"
#include "GL/glew.h"
#include "../TextureManager.h"
#include "Animation.h"
#include "MovingAnimation.h"
#include <map>
#include "../AssetManager.h"
// TODO: (extra): can add states for different states (0 for full solid tile or 1 for no solid
class SpriteComponent : public Component //sprite -> transform
{
private:
	SDL_Texture *texture;
	GLuint _vboID = 0; //32 bits
public:
	TransformComponent* transform;
	SDL_Rect srcRect, destRect;
	Vector2D tempPosition;

	Animation animation;
	MovingAnimation moving_animation;
	
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, Vector2D pos)
	{
		setTex(id);
		tempPosition = pos;
	}

	~SpriteComponent()
	{
		if (_vboID != 0) //buffer hasn't been generated
		{
			glDeleteBuffers(1, &_vboID); // create buffer and change vboID to point to that buffer
		}
		//SDL_DestroyTexture(texture); //no need for that anymore, because sprite points to a texture that could be used by multiple objects
	}

	void setTex(std::string id) //this function is used to change texture of a sprite
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
		
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;

		// OPENGL tutorial

		if (_vboID == 0) //buffer hasn't been generated
		{
			glGenBuffers(1, &_vboID); // create buffer and change vboID to point to that buffer
		}

		float vertexData[12]; // 6 vertices * the coordinates
		
		//First triangle
		vertexData[0] = transform->position.x + transform->width;
		vertexData[1] = transform->position.y + transform->height;

		vertexData[2] = transform->position.x;
		vertexData[3] = transform->position.y + transform->height;

		vertexData[4] = transform->position.x;
		vertexData[5] = transform->position.y;
		//Second triangle
		vertexData[6] = transform->position.x;
		vertexData[7] = transform->position.y;

		vertexData[8] = transform->position.x + transform->width;
		vertexData[9] = transform->position.y;

		vertexData[10] = transform->position.x + transform->width;
		vertexData[11] = transform->position.y + transform->height;

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); // GL_STATIC_DRAW -> draw once

		glBindBuffer(GL_ARRAY_BUFFER, 0); // dont have to do this, for safety
	}

	void update() override
	{
		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		glEnableVertexAttribArray(0); // give positions ( point to 0 element for position)

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // tell what data it is (first 0) and where the data is ( last 0 to go from the beggining)

		glDrawArrays(GL_TRIANGLES, 0, 6); // draw arrays (0 for where is first element, 6 vertices for one square)

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SetAnimation(int idX, int idY, int fr, float sp, const Animation::animType type, int reps = 0)
	{
		animation = Animation(idX, idY, fr, sp, type, reps);
	}

	void SetMovingAnimation(int idX, int idY, int fr, float sp, const Animation::animType type, int dx, int dy, int reps = 0)
	{
		moving_animation = MovingAnimation(idX, idY, fr, sp, type, dx, dy, reps);
	}

	void setCurrFrame() {
		this->srcRect.x = (this->animation.indexX * this->transform->width) /* init */ + ( this->srcRect.w * animation.cur_frame_index/* curframe from total frams */);
		this->srcRect.y = this->animation.indexY * this->transform->height;
	}

	void setMoveFrame() {
		this->destRect.x = (static_cast<int>(this->transform->position.x) - Game::camera.x + this->moving_animation.indexX) /* init */ + (this->moving_animation.distanceX * moving_animation.cur_frame_index);
		this->destRect.y = (static_cast<int>(this->transform->position.y) - Game::camera.y + this->moving_animation.indexY) + (this->moving_animation.distanceY * moving_animation.cur_frame_index);
	}

	void DestroyTex()
	{
		//TextureManager::DestroyTexture(texture);
		texture = NULL;
	}
};
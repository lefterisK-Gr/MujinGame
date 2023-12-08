#pragma once

#include "Components.h"
#include <SDL/SDL.h>
#include "GL/glew.h"
#include "../TextureManager.h"
#include "Animation.h"
#include "MovingAnimation.h"
#include <map>
#include "../AssetManager.h"
#include "../Vertex.h"
#include <cstddef>

// TODO: (extra): can add states for different states (0 for full solid tile or 1 for no solid
class SpriteComponent : public Component //sprite -> transform
{
private:
	const GLTexture *gl_texture;
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
		//texture = Game::assets->GetTexture(id);
		gl_texture = Game::assets->Get_GLTexture(id);
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

		Vertex vertexData[6]; // 6 vertices * the coordinates
		
		//First triangle
		vertexData[0].setPosition(transform->position.x + 0.5f, transform->position.y + 0.5f);
		vertexData[0].setUV(1.0f, 1.0f);
		vertexData[1].setPosition(transform->position.x, transform->position.y + 0.5f);
		vertexData[1].setUV(0.0f, 1.0f);
		vertexData[2].setPosition(transform->position.x, transform->position.y);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second triangle
		vertexData[3].setPosition(transform->position.x, transform->position.y);
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPosition(transform->position.x + 0.5f, transform->position.y);
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPosition(transform->position.x + 0.5f, transform->position.y + 0.5f);
		vertexData[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); // GL_STATIC_DRAW -> draw once

		glBindBuffer(GL_ARRAY_BUFFER, 0); // dont have to do this, for safety
	}

	void update() override
	{
		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x; //make player move with the camera, being stable in centre, except on edges
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;

		//float gl_position_x = 2.0f * (destRect.x - 400.0f) / 800.0f;
		//float gl_position_y = -(2.0f * (destRect.y - 320.0f) / 640.0f);
		//if (gl_position_x > -1 && gl_position_x < 1) {
		//	std::cout << "seeing sprite positions";
		//}
		float gl_position_x = 0.f;
		float gl_position_y = 0.0f;

		float gl_sprite_width = 32/*(static_cast<float>(transform->width) / 800)*/;
		float gl_sprite_height = 32/*(static_cast<float>(transform->height) / 640)*/;
			
		if (_vboID == 0) //buffer hasn't been generated
		{
			glGenBuffers(1, &_vboID); // create buffer and change vboID to point to that buffer
		}

		Vertex vertexData[6]; // 6 vertices * the coordinates
		transform->position.x += 0.01f;
		//First triangle
		vertexData[0].setPosition(gl_position_x + gl_sprite_width, gl_position_y + gl_sprite_height);
		vertexData[0].setUV(1.0f, 1.0f);
		vertexData[1].setPosition(gl_position_x, gl_position_y + gl_sprite_height);
		vertexData[1].setUV(0.0f, 1.0f);
		vertexData[2].setPosition(gl_position_x, gl_position_y);
		vertexData[2].setUV(0.0f, 0.0f);

		//Second triangle
		vertexData[3].setPosition(gl_position_x, gl_position_y);
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPosition(gl_position_x + gl_sprite_width, gl_position_y);
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPosition(gl_position_x + gl_sprite_width, gl_position_y + gl_sprite_height);
		vertexData[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

	/*	vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);*/

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); // GL_STATIC_DRAW -> draw once

		glBindBuffer(GL_ARRAY_BUFFER, 0); // dont have to do this, for safety
	}

	void draw() override
	{
		//TextureManager::Draw(texture, srcRect, destRect, spriteFlip);

		//bind texture
		glBindTexture(GL_TEXTURE_2D, gl_texture->id);
		/*glActiveTexture(GL_TEXTURE0);
		const GLTexture* terrain_Texture = assets->Get_GLTexture("terrain");
		glBindTexture(GL_TEXTURE_2D, (*terrain_Texture).id);
		GLint textureLocation = _colorProgram.getUniformLocation("texture_sampler");
		glUniform1i(textureLocation, 0);*/

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		glEnableVertexAttribArray(0); // give positions ( point to 0 element for position)

		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // tell what data it is (first 0) and where the data is ( last 0 to go from the beggining)
		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6); // draw arrays (0 for where is first element, 6 vertices for one square)

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/*glBindTexture(GL_TEXTURE_2D, 0);*/
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
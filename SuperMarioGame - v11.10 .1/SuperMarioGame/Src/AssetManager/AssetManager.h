#pragma once

#include <map>
#include <string>
#include "../TextureManager.h"
#include "../ShapeManager.h"
#include "../Vector2D.h"
#include "../ECS\ECS.h"
#include <TTF/SDL_ttf.h>


class AssetManager //this class created when we added projectiles, based on this class other components changed
{					//it just replaces the paths of textures with names
public:
	SDL_Color black = { 0, 0 ,0 ,255 };
	SDL_Color white = { 255, 255 ,255 ,255 };
	SDL_Color red = { 255, 0 ,0 ,255 };
	SDL_Color green = { 0, 255 ,0 ,255 };

	AssetManager(Manager* man);
	~AssetManager();

	//gameobjects
	void ProjectileExplosion(int camerapos);
	void CreatePlayer(Entity& player);
	void CreateSunShape(Entity& sun);
	void CreateProjectile(Vector2D pos, Vector2D vel,int range, int speed, std::string id);
	void CreateSkeleton(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
	void CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
	void ActivateEnemy(Entity& enemy);

	//triggers
	bool OnPipeTrigger(SDL_Rect collider);
	bool LeftOfPipeTrigger(SDL_Rect collider);

	//sprite management

	//texture management
	void Add_GLTexture(std::string id, const char* path);
	const GLTexture* Get_GLTexture(std::string id);

private:
	Manager* manager;

	std::map<std::string, const GLTexture*> gl_textures;

	SDL_Rect onpipeTriggers[2];
	SDL_Rect leftofpipeTriggers[1];
};
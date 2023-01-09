#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS\ECS.h"
#include "SDL_ttf.h"

class AssetManager //this class created when we added projectiles, based on this class other components changed
{					//it just replaces the paths of textures with names
public:
	AssetManager(Manager* man);
	~AssetManager();

	//gameobjects
	void ProjectileExplosion(int camerapos);
	void CreatePlayerComponents(Entity& player);
	void CreateProjectile(Vector2D pos, Vector2D vel,int range, int speed, std::string id);
	void CreateGoomba(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
	void CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
	void ActivateEnemy(Entity& enemy);

	//triggers
	bool OnPipeTrigger(SDL_Rect collider);
	bool LeftOfPipeTrigger(SDL_Rect collider);

	//sprite management
	void AddPipeRingForegroundTile(int srcX, int srcY, Vector2D pos);

	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);
private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	SDL_Rect onpipeTriggers[2];
	SDL_Rect leftofpipeTriggers[1];
};
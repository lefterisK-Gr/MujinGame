#pragma once

#include <string>
#include "TextureManager/TextureManager.h"
#include "../ShapeManager.h"
#include "Vector2D/Vector2D.h"
#include "ECS\ECS.h"
#include <TTF/SDL_ttf.h>
#include "../Game.h"


class AssetManager //this class created when we added projectiles, based on this class other components changed
{					//it just replaces the paths of textures with names
public:
	SDL_Color black = { 0, 0 ,0 ,255 };
	SDL_Color white = { 255, 255 ,255 ,255 };
	SDL_Color red = { 255, 0 ,0 ,255 };
	SDL_Color green = { 0, 255 ,0 ,255 };

	AssetManager(Manager* man, InputManager& inputManager);
	~AssetManager();

	//gameobjects
	void ProjectileExplosion(int camerapos);
	void CreatePlayer(Entity& player);
	void CreateBackground();
	void CreateSunShape(Entity& sun);
	void CreateRandomParticlesGenerator();
	void CreateProjectile(Vector2D pos, Vector2D vel,int range, int speed, std::string id);
	void CreateSkeleton(Vector2D pos, Vector2D vel, std::string id, bool isGiant);
	void CreateStageUpButtons();
	void CreateShop();
	void RefreshShop();
	void CreateInventory();
	void CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int speed, std::string id);

	void CreateEnemies();

	//triggers
	bool OnPipeTrigger(SDL_Rect collider);
	bool LeftOfPipeTrigger(SDL_Rect collider);

private:
	Manager* manager;
	InputManager& _inputManager;

	SDL_Rect onpipeTriggers[2];
	SDL_Rect leftofpipeTriggers[1];
};
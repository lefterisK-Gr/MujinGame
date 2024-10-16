#pragma once

#include <string>
#include "TextureManager/TextureManager.h"
#include "../ShapeManager.h"
#include "ECS\ECSManager.h"
#include <TTF/SDL_ttf.h>
#include "../Game.h"


class AssetManager //this class created when we added projectiles, based on this class other components changed
{					//it just replaces the paths of textures with names
public:
	enum class Weather {
		CLEAR,
		RAIN,
		SNOW
	};
	SDL_Color black = { 0, 0 ,0 ,255 };
	SDL_Color white = { 255, 255 ,255 ,255 };
	SDL_Color red = { 255, 0 ,0 ,255 };
	SDL_Color green = { 0, 255 ,0 ,255 };

	AssetManager(Manager* man, InputManager& inputManager, MujinEngine::Window& window);
	~AssetManager();

	//gameobjects
	void ProjectileExplosion(int camerapos);
	void CreatePlayer(Entity& player);
	void CreateBackground();
	void CreateRandomParticlesGenerator();
	void CreateRain(Entity& entity);
	void CreateSnow(Entity& entity);
	void CreateProjectile(glm::ivec2 pos, glm::ivec2 vel,int range, int speed, std::string id);
	void CreateSkeleton(glm::ivec2 pos, glm::ivec2 vel, std::string id, bool isGiant);
	void CreateShop();
	void RefreshShop();
	void CreateInventory();
	void CreateFog();
	void CreateWeather(Entity& entity);
	void SetBackGroundColor(float r, float g, float b, float a);
	Weather getWeather() const {
		return _weather;
	}
	void CreateGreenKoopaTroopa(glm::ivec2 pos, glm::ivec2 vel, int speed, std::string id);

	void CreateEnemies();

	Entity& CreateGem(TransformComponent* target_tr);

	//triggers
	bool OnPipeTrigger(SDL_Rect collider) const;
	bool LeftOfPipeTrigger(SDL_Rect collider) const;

private:
	Weather _weather;
	Manager* manager;
	InputManager& _inputManager;
	MujinEngine::Window& _window;

	SDL_Rect onpipeTriggers[2];
	SDL_Rect leftofpipeTriggers[1];
};
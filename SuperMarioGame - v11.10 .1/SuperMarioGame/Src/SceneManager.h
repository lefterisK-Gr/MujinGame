#pragma once

#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS\ECS.h"

class SceneManager //this class created when we added projectiles, based on this class other components changed
{					//it just replaces the paths of textures with names
public:
	int sceneSelected = 0;
	SceneManager();
	~SceneManager();

	Vector2D GetScenePosition(int i);
	SDL_Rect GetSceneCamera(int i);
private:
	Vector2D transitionScenePositions[2];
	SDL_Rect sceneCameras[2];
};
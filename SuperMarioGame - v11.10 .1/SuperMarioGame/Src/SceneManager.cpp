#include "SceneManager.h"
#include "ECS/Components.h"

SceneManager::SceneManager()
{
	transitionScenePositions[0].x = 2608;
	transitionScenePositions[0].y = 416;

	transitionScenePositions[1].x = 3088;
	transitionScenePositions[1].y = 33;

	sceneCameras[0].x = 0;
	sceneCameras[0].y = 0;
	sceneCameras[0].w = 2240;
	sceneCameras[0].h = 0;

	sceneCameras[1].x = 3040;
	sceneCameras[1].y = 0;
	sceneCameras[1].w = 0;
	sceneCameras[1].h = 0;
}

SceneManager::~SceneManager()
{}

Vector2D SceneManager::GetSceneStartupPosition(int i)
{
	return transitionScenePositions[i];
}

SDL_Rect SceneManager::GetSceneCamera(int i)
{
	return sceneCameras[i];
}
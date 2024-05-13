#include "SceneManager.h"

SceneManager::SceneManager() // todo make structure like animationManager with names for scenes
{
	transitionScenePositions.push_back(Vector2D(64, 33));
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

void SceneManager::AddSceneCamera(SDL_Rect scene)
{
	sceneCameras.push_back(scene);
}
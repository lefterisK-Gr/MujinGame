#pragma once
#include <SDL/SDL.h>
#include "ECS/Components.h"
#include "Vector2D/Vector2D.h"
#include "Window/Window.h"
#include <algorithm>

class Collision;

class Collision
{
public:
	enum ColSide { NONE = 0, TOP = 1, RIGHT = 2, DOWN = 3, LEFT = 4 };

	ColSide movingRectColSide;

	SDL_Rect storedColliderRect;
	Vector2D overlap;
	bool isSidewaysCollision = false;
	bool isCollision = false;

	static bool checkCollision(const SDL_Rect recA, const SDL_Rect recB);
	bool checkCollisionIsSideways(const SDL_Rect& moving_recA, const SDL_Rect& recB);
	void moveFromCollision(Entity& player);
	bool moveFromOuterBounds(Entity& entity, MujinEngine::Window& window);
};
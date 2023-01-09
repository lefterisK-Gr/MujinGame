#pragma once
#include <SDL.h>
#include "Vector2D.h"

enum CollisionDirections { COL_NONE= 0, COL_TOP_LEFT= 1, COL_TOP_RIGHT= 2, COL_DOWN_LEFT= 4, COL_DOWN_RIGHT = 8};

class ColliderComponent;

class Collision
{
public:
	static int countCollisions;
	static Vector2D storedCollider;
	static bool hittedTopLeft;
	static bool hittedTopRight;
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static void checkCollision(Vector2D &playerVel, Vector2D &playerPos, bool &rigidBodyOnGround, bool colHasCollAbove);
};
#pragma once
#include <SDL.h>
#include "ECS/Components.h"
#include "Vector2D.h"

enum CollisionDirections { COL_NONE= 0, COL_TOP_LEFT= 1, COL_TOP_RIGHT= 2, COL_DOWN_LEFT= 4, COL_DOWN_RIGHT = 8};

class ColliderComponent;

class Collision
{
public:
	enum ColSide { NONE = 0, TOP = 1, RIGHT = 2, DOWN = 3, LEFT = 4 };

	ColSide movingRectColSide;

	static int countCollisions;
	static Vector2D storedCollider;

	SDL_Rect storedColliderRect;
	Vector2D dist;
	bool isSidewaysCollision;
	bool isCollision;

	static bool hittedTopLeft;
	static bool hittedTopRight;

	bool checkCollision(const SDL_Rect& moving_recA, const SDL_Rect& recB);
	void moveFromCollision(Entity& player);

	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static void checkCollision(Vector2D &playerVel, Vector2D &playerPos, bool &rigidBodyOnGround, bool colHasCollAbove);
};
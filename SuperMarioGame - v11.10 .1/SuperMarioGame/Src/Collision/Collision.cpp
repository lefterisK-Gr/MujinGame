#include "Collision.h"
#include "../ECS\ColliderComponent.h"


bool Collision::checkCollision(const SDL_Rect recA, const SDL_Rect recB) {
	if (recA.x >= recB.x + recB.w || recA.x + recA.w <= recB.x ||
		recA.y >= recB.y + recB.h || recA.y + recA.h <= recB.y) {
		return false; // no collision
	}
	return true;
}

bool Collision::checkCollisionIsSideways(const SDL_Rect& moving_recA, const SDL_Rect& recB) {

	if (!Collision::checkCollision(moving_recA, recB)) {
		return false;
	}

	Collision::storedColliderRect = recB;

	//calculate distX,distY
	Collision::dist.x = (recB.x + (recB.w / 2) ) - (moving_recA.x + (moving_recA.w / 2)); //positive if collider on right, negative if collider on left of center
	Collision::dist.y = (recB.y + (recB.h / 2) ) - (moving_recA.y + (moving_recA.h / 2));

	if (moving_recA.h > moving_recA.w) {
		if (dist.y <= -(moving_recA.h - moving_recA.w)) {
			Collision::dist.y = Collision::dist.y + ((moving_recA.h - moving_recA.w) / 2);
		}
		else if (dist.y >= (moving_recA.h - moving_recA.w))
		{
			Collision::dist.y = Collision::dist.y - ((moving_recA.h - moving_recA.w) / 2);
		}
	}

	//check if it is sideways collision (bool isSidewaysCollision)
	if (abs(dist.x) > abs(dist.y)) { //todo: if velocity.y < 0 then have it >= to slide on left wall
		Collision::isSidewaysCollision = true;
		Collision::movingRectColSide = (dist.x > 0) ? Collision::ColSide::RIGHT : Collision::ColSide::LEFT;
	}
	else { // Vertical collision
		Collision::movingRectColSide = (dist.y > 0) ? Collision::ColSide::DOWN : Collision::ColSide::TOP;
	}

	Collision::isCollision = true;
	return true;
}

void Collision::moveFromCollision(Entity& player) {
	auto& playerTransform = player.getComponent<TransformComponent>();
	auto playerCollider = player.getComponent<ColliderComponent>();

	switch (Collision::movingRectColSide) {
		case Collision::ColSide::RIGHT: // Move player to the left of the collider
			playerTransform.position.x = storedColliderRect.x - playerCollider.collider.w - (2 * playerTransform.scale * 8) - 1;
			break;

		case Collision::ColSide::LEFT: // Move player to the right of the collider
			playerTransform.position.x = storedColliderRect.x - ((playerTransform.scale) * 8);
			break;

		case Collision::ColSide::DOWN: // Move player above the collider
			playerTransform.position.y = storedColliderRect.y - (playerTransform.height * playerTransform.scale);
			player.getComponent<RigidBodyComponent>().onGround = true;
			break;

		case Collision::ColSide::TOP: // Move player below the collider
			playerTransform.position.y = storedColliderRect.y - ((playerTransform.scale - 1) * 8);
			playerTransform.velocity.y = 0;
			break;

		default:
			break;
	}
	player.getComponent<ColliderComponent>().update(1.0f);
}

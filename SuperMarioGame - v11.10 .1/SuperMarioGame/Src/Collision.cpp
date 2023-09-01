#include "Collision.h"
#include "ECS\ColliderComponent.h"


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
	}

	Collision::isCollision = true;
	return true;
}

void Collision::moveFromCollision(Entity& player) {
	auto& playerTransform = player.getComponent<TransformComponent>();
	auto playerCollider = player.getComponent<ColliderComponent>().collider;

	if (Collision::isSidewaysCollision) { //horizontal move
		if (Collision::dist.x > 0) { //move moving_rect left from collider
			playerTransform.position.x = storedColliderRect.x - playerCollider.w - (playerTransform.scale * 8) - 1;
			Collision::movingRectColSide = Collision::ColSide::RIGHT;
		}
		else {
			playerTransform.position.x = storedColliderRect.x - ((playerTransform.scale - 1) * 8); //consider position based on collider
			Collision::movingRectColSide = Collision::ColSide::LEFT;
		}
	}
	else {
		if (Collision::dist.y > 0) { //move moving_rect down from collider
			playerTransform.position.y = storedColliderRect.y - (playerTransform.height * playerTransform.scale);
			player.getComponent<RigidBodyComponent>().onGround = true;
			Collision::movingRectColSide = Collision::ColSide::DOWN;
		}
		else {
			playerTransform.position.y = storedColliderRect.y - ((playerTransform.scale - 1) * 8);
			Collision::movingRectColSide = Collision::ColSide::TOP;
			playerTransform.velocity.y = 0;
		}
	}
	player.getComponent<ColliderComponent>().update();
}

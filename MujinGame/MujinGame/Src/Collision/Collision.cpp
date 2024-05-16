#include "Collision.h"


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

	Collision::overlap.x = min(moving_recA.x + moving_recA.w, recB.x + recB.w) - max(moving_recA.x, recB.x);
	Collision::overlap.y = min(moving_recA.y + moving_recA.h, recB.y + recB.h) - max(moving_recA.y, recB.y);

	//check if it is sideways collision (bool isSidewaysCollision)
	if (Collision::overlap.x < Collision::overlap.y) { //todo: if velocity.y < 0 then have it >= to slide on left wall
		Collision::isSidewaysCollision = true;
		Collision::movingRectColSide = (moving_recA.x < recB.x) ? Collision::ColSide::RIGHT : Collision::ColSide::LEFT;
	}
	else { // Vertical collision
		Collision::movingRectColSide = (moving_recA.y > recB.y) ?  Collision::ColSide::TOP : Collision::ColSide::DOWN;
	}

	Collision::isCollision = true;
	return true;
}

void Collision::moveFromCollision(Entity& player) {
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto playerCollider = player.GetComponent<ColliderComponent>();

	switch (Collision::movingRectColSide) {
		case Collision::ColSide::RIGHT: // Move player to the left of the collider
			playerTransform.position.x -= Collision::overlap.x;
			break;

		case Collision::ColSide::LEFT: // Move player to the right of the collider
			playerTransform.position.x += Collision::overlap.x;
			break;

		case Collision::ColSide::DOWN: // Move player above the collider
			playerTransform.position.y -= Collision::overlap.y;
			player.GetComponent<RigidBodyComponent>().onGround = true;
			break;

		case Collision::ColSide::TOP: // Move player below the collider
			playerTransform.position.y += Collision::overlap.y;
			playerTransform.velocity.y = 0;
			break;

		default:
			break;
	}
}

bool Collision::moveFromOuterBounds(Entity& entity, MujinEngine::Window& window) {
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

	auto& pos = entity.GetComponent<TransformComponent>().position;

	if (pos.x < 0) {
		pos.x = 0;
		return true;
	}
	else if (pos.x + entity.GetComponent<TransformComponent>().width > main_camera2D->worldLocation.w + window.getScreenWidth()) {
		pos.x = main_camera2D->worldLocation.w + window.getScreenWidth() - entity.GetComponent<TransformComponent>().width;
		return true;
	}
	return false;
}

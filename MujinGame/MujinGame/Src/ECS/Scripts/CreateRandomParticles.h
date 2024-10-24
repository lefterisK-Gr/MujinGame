#pragma once

#include "ECS/Components.h"

class CreateRandomParticles : public Component
{
private:
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));

	const int maxParticles = 20;
	float numOfParticles = 0;
	float spawnRate = 0.1f;
	std::default_random_engine generator;  // Random number generator
	std::uniform_real_distribution<float> distributionY;
	std::uniform_real_distribution<float> distributionVX; // Distribution for X velocity
	std::uniform_real_distribution<float> distributionVY;
public:

	CreateRandomParticles(MujinEngine::Window& window) :
		distributionY(0, main_camera2D->getCameraDimensions().y),
		distributionVX(-0.2, 0.2),
		distributionVY(-0.2, 0.2) 
	{
		generator.seed(std::random_device{}());
	}

	~CreateRandomParticles() {

	}

	void init() override {

	}

	void update(float deltaTime) override {
		auto& texturelights(manager.getGroup(Manager::groupTextureLights));
		auto& players(manager.getGroup(Manager::groupPlayers));

		for (auto& pl : players) {
			TransformComponent* tr = &pl->GetComponent<TransformComponent>();
			std::uniform_real_distribution<float> distributionX(tr->getPosition().x - main_camera2D->getCameraDimensions().x,
				tr->getPosition().x + main_camera2D->getCameraDimensions().x);
			const unsigned short prevNumOfParticles = static_cast<unsigned short>(numOfParticles);

			if (numOfParticles + spawnRate <= maxParticles) {
				numOfParticles += spawnRate;
			}
			if (static_cast<unsigned short>(numOfParticles) > prevNumOfParticles && numOfParticles < maxParticles) {
				Entity* light;
				light = &manager.addEntity();

				glm::vec2 randPos(distributionX(generator), distributionY(generator));
				glm::vec2 randVel(distributionVX(generator), distributionVY(generator));

				TransformComponent& tc = light->addComponent<TransformComponent>(randPos);
				tc.setVelocity_X(randVel.x);
				tc.setVelocity_Y(randVel.y);

				light->addComponent<SpriteComponent>(Color(255, 255, 255, 200));
				light->addComponent<LightTextureComponent>(1);
				FlashAnimatorComponent& fc = light->addComponent<FlashAnimatorComponent>();
				fc.Play("RandomParticle");
				light->addGroup(Manager::groupTextureLights);
			}

			for (auto& tl : texturelights) {
				if (tl->GetComponent<SpriteComponent>().flash_animation.hasFinished()) {
					tl->destroy();
					numOfParticles--;
				}
			}
		}
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
	}
};
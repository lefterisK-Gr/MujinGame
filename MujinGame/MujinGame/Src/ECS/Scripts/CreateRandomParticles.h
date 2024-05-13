#pragma once

#include "ECS/Components.h"

class CreateRandomParticles : public Component
{
private:
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

	const int maxParticles = 20;
	int numOfParticles = 0;
	std::default_random_engine generator;  // Random number generator
	std::uniform_int_distribution<int> distributionX;
	std::uniform_int_distribution<int> distributionY;
	std::uniform_real_distribution<float> distributionVX; // Distribution for X velocity
	std::uniform_real_distribution<float> distributionVY;
public:

	CreateRandomParticles(MujinEngine::Window& window) :
		distributionX(0, main_camera2D->worldLocation.w + window.getScreenWidth()),
		distributionY(0, main_camera2D->getCameraDimensions().y),
		distributionVX(-0.5, 0.5),
		distributionVY(-0.5, 0.5) {
		generator.seed(std::random_device{}());
	}

	~CreateRandomParticles() {

	}

	void init() override {

	}

	void update(float deltaTime) override {
		auto& texturelights(manager.getGroup(Manager::groupTextureLights));

		if (numOfParticles < maxParticles) {
			Entity* light;
			light = &manager.addEntity();

			int randX = distributionX(generator);  // Generate random X within camera width
			int randY = distributionY(generator);
			float randVX = distributionVX(generator);
			float randVY = distributionVY(generator);

			TransformComponent& tc = light->addComponent<TransformComponent>(randX, randY);
            tc.velocity = Vector2D(randVX, randVY);

			light->addComponent<SpriteComponent>(Color(255, 255, 255, 150));
			light->addComponent<LightTextureComponent>(1);
			FlashAnimatorComponent& fc = light->addComponent<FlashAnimatorComponent>();
			fc.Play("RandomParticle");
			light->addGroup(Manager::groupTextureLights);
			numOfParticles++;
		}

		for (auto& tl : texturelights) {
			if (tl->GetComponent<SpriteComponent>().flash_animation.hasFinished()) {
				tl->destroy();
				numOfParticles--;
			}
		}
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
	}
};
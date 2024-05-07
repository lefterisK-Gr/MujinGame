#pragma once

#include "ECS/Components.h"

class CreateRandomParticles : public Component
{
private:

	const int maxParticles = 10;
	int numOfParticles = 0;
	std::default_random_engine generator;  // Random number generator
	std::uniform_int_distribution<int> distributionX;
	std::uniform_int_distribution<int> distributionY;
	std::uniform_real_distribution<float> distributionVX; // Distribution for X velocity
	std::uniform_real_distribution<float> distributionVY;
public:

	CreateRandomParticles() :
		distributionX(0, Game::camera.w),
		distributionY(0, Game::camera2D.getCameraDimensions().y),
		distributionVX(-2.0, 2.0),
		distributionVY(-2.0, 2.0) {
		generator.seed(std::random_device{}());
	}

	~CreateRandomParticles() {

	}

	void init() override {

	}

	void update(float deltaTime) override {
		auto& texturelights(manager.getGroup(Game::groupTextureLights));

		if (numOfParticles < maxParticles) {
			Entity* light;
			light = &manager.addEntity();

			int randX = distributionX(generator);  // Generate random X within camera width
			int randY = distributionY(generator);
			float randVX = distributionVX(generator);
			float randVY = distributionVY(generator);

			TransformComponent& tc = light->addComponent<TransformComponent>(randX, randY);
            tc.velocity = Vector2D(randVX, randVY);

			light->addComponent<LightTextureComponent>(1);
			light->GetComponent<SpriteComponent>().default_color = Color(0, 0, 255, 150);
			FlashAnimatorComponent& fc = light->addComponent<FlashAnimatorComponent>();
			fc.Play("RandomParticle");
			light->addGroup(Game::groupTextureLights);
			numOfParticles++;
		}

		for (auto& tl : texturelights) {
			if (tl->GetComponent<SpriteComponent>().flash_animation.hasFinished()) {
				tl->destroy();
				numOfParticles--;
			}
		}
	}

	void draw(SpriteBatch& batch) override {
	}
};
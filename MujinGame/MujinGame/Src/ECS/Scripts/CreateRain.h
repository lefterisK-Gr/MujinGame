#pragma once

#include "ECS/Components.h"

class CreateRainDrops : public Component
{
private:
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

	const int maxParticles = 20;
	int numOfParticles = 0;
	std::default_random_engine generator;  // Random number generator
	std::uniform_int_distribution<int> distributionX;
public:

	CreateRainDrops(MujinEngine::Window& window) :
		distributionX(0, main_camera2D->worldLocation.w + window.getScreenWidth())
	{
		generator.seed(std::random_device{}());
	}

	~CreateRainDrops() {

	}

	void init() override {

	}

	void update(float deltaTime) override {
		auto& textureraindrop(manager.getGroup(Manager::groupRainDrop));

		if (numOfParticles < maxParticles) {
			Entity* raindrop;
			raindrop = &manager.addEntity();

			int randX = distributionX(generator);  // Generate random X within camera width

			TransformComponent& tc = raindrop->addComponent<TransformComponent>(100, 0, 100, 100, 1);
			tc.velocity = Vector2D(0, 1);

			raindrop->addComponent<Rectangle_w_Color>();
			raindrop->GetComponent<Rectangle_w_Color>().color = Color(128, 128, 128, 255);
			raindrop->addGroup(Manager::groupRainDrop);
			numOfParticles++;
		}

		for (auto& tr : textureraindrop) {
			if (tr->GetComponent<TransformComponent>().position.y > main_camera2D->getCameraDimensions().y) {
				tr->destroy();
				numOfParticles--;
			}
		}
	}

	void draw(SpriteBatch& batch, MujinEngine::Window& window) override {
	}
};
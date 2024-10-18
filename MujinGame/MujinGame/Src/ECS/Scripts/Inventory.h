#pragma once

#include "ECS/Components.h"

class Inventory : public Component
{
private:
	std::vector<Entity*> _slots;
public:

	#define SLOT_SIZE 64.0f
	#define SLOT_SPACE 40.0f
	#define SLOTS_PER_ROW 5
	#define SLOTS_PER_COLUMN 3

	bool isOpen = false;

	TransformComponent* transform = nullptr;
	Rectangle_w_Color* rectangle = nullptr;

	Inventory()
	{

	}

	~Inventory() {
		for (auto& s : _slots)
		{
			s->destroy();
		}
		_slots.clear();
	}

	void init() override {
		std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));

		if (!entity->hasComponent<TransformComponent>()) {
			glm::ivec2 inventoryDims = glm::ivec2(600, 400);
			entity->addComponent<TransformComponent>(
				glm::vec2(
					main_camera2D->getCameraDimensions().x / 2 - inventoryDims.x / 2,
					main_camera2D->getCameraDimensions().y / 2 - inventoryDims.y / 2
				),
				Manager::actionLayer, inventoryDims, 1);
		}
		transform = &entity->GetComponent<TransformComponent>();
		if (!entity->hasComponent<Rectangle_w_Color>()) {
			entity->addComponent<Rectangle_w_Color>();
		}
		rectangle = &entity->GetComponent<Rectangle_w_Color>();
		rectangle->color = Color(200, 200, 200, 255);

		for (auto i = 0; i < SLOTS_PER_COLUMN; i++)
		{
			for (auto j = 0; j < SLOTS_PER_ROW; j++)
			{
				auto& slot = manager.addEntity();
				slot.addComponent<TransformComponent>(glm::vec2(0.0f, 0.0f), Manager::actionLayer, glm::ivec2(64, 64), transform->scale);
				slot.addComponent<Rectangle_w_Color>();
				slot.GetComponent<Rectangle_w_Color>().color = Color(128, 128, 128, 255); // Grey color
				slot.addGroup(Manager::groupSlots);
				_slots.emplace_back(&slot);
			}
		}
	}

	void update(float deltaTime) override {
		std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));

		transform->setPosition_X(isOpen ? main_camera2D->getCameraDimensions().x / 2 - transform->width / 2 : -1000.0f);
		
		for (int i = 0; i < SLOTS_PER_COLUMN; i++) {
			for (int j = 0; j < SLOTS_PER_ROW; j++) {
				int index = i * SLOTS_PER_ROW + j; // Calculate index in the flat vector
				if (index < _slots.size()) {
					auto& slot = _slots[index];
					// Calculate position based on row and column
					float posX = transform->getPosition().x + (SLOT_SIZE + SLOT_SPACE) * j;
					float posY = transform->getPosition().y + (SLOT_SIZE + SLOT_SPACE) * i;

					slot->GetComponent<TransformComponent>().setPosition_X(posX + 50.0f);
					slot->GetComponent<TransformComponent>().setPosition_Y(posY + 50.0f);
				}
			}
		}
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
	}
};
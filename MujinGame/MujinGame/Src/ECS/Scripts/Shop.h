#pragma once

#include "ECS/Components.h"
#include "Item.h"

class Shop : public Component
{
private:
	std::vector<Entity*> _items;
public:

	bool isOpen = false;

	TransformComponent* transform = nullptr;
	Rectangle_w_Color* rectangle = nullptr;

	Shop()
	{

	}

	~Shop() {
		for (auto& s : _items)
		{
			s->destroy();
		}
		_items.clear();

		//// Assuming manager.getGroup returns a reference to the group
		//auto& items = manager.getGroup(Manager::groupShopItems);
		//auto& itemsback = manager.getGroup(Manager::groupShopItemsBack);

		//// Clear the entities from the groups
		//items.clear();
		//itemsback.clear();
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(glm::vec2(50.0f, 50.0f), glm::ivec2(400, 600), 1);
		}
		transform = &entity->GetComponent<TransformComponent>();
		if (!entity->hasComponent<Rectangle_w_Color>()) {
			entity->addComponent<Rectangle_w_Color>();
		}
		rectangle = &entity->GetComponent<Rectangle_w_Color>();
		rectangle->color = Color(100, 100, 100, 255);
	}

	void update(float deltaTime) override {
		transform->setPosition_X(isOpen ? 0 : -1000) ;

		float currentX = transform->getPosition().x + 100.0f;
		float itemPositionIncrementX = 100.0f;

		for (auto& s : _items)
		{
			s->GetComponent<TransformComponent>().setPosition_X(currentX);
			currentX += itemPositionIncrementX;
		}
	}

	void draw(SpriteBatch&  batch, MujinEngine::Window& window) override {
	}

	void generateRandomItems() {
		_items.clear();

		auto& item1(manager.addEntity());
		auto& item2(manager.addEntity());
		auto& item3(manager.addEntity());

		item1.addComponent<Item>("sword", 100);
		item1.addComponent<ButtonComponent>(std::bind(&Shop::handleBuyItem, this));
		item1.addGroup(Manager::groupStageUpAttackButtons);

		item2.addComponent<Item>("shield", 200);
		item2.addComponent<ButtonComponent>(std::bind(&Shop::handleBuyItem, this));
		item2.addGroup(Manager::groupStageUpDefenceButtons);

		item3.addComponent<Item>("healthPotion", 300);
		item3.addComponent<ButtonComponent>(std::bind(&Shop::handleBuyItem, this));
		item3.addGroup(Manager::groupStageUpHpButtons);

		// Generate 3 random items
		_items.push_back(&item1); // Example item
		_items.push_back(&item2);  // Another example
		_items.push_back(&item3);  // Yet another example

		// Consider adding more randomness here, like randomizing names, types, or prices
	}

	void handleBuyItem() {

	}
};
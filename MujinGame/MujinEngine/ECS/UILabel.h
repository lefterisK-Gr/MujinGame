#pragma once

#include "../PNG_Letters.h"
#include "Components.h"

class Manager;

class UILabel : public Component
{
private:
	Manager* _manager;
	std::vector<Entity*> letters;
	std::string label;
	std::string fontFamily;
	bool isHud = false;
public:
	TransformComponent* transform = nullptr;

	UILabel() = default;
	UILabel(Manager* manager, std::string lab, std::string fontFam, bool is_hud)
	{
		_manager = manager;
		isHud = is_hud;
		fontFamily = fontFam;
		label = lab;
	}

	~UILabel() {

	}

	void init() override {
		//create entities for each letter
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->GetComponent<TransformComponent>();
		setLetters(label);
	}

	void update(float deltaTime) override {
		//if string changes then for all the labels that have been created,
		//find the ones that are for that string and delete them?
		if (isHud) {
			int previousCharX = 0;

			for (auto& l : letters) {
				l->GetComponent<TransformComponent>().setPosition_X(transform->getPosition().x + previousCharX) ;
				previousCharX += l->GetComponent<TransformComponent>().width;
			}
		}
	}

	void draw(SpriteBatch& batch, MujinEngine::Window& window) override {
		//draw each letter
		for (auto& l : letters) {
			l->draw(batch, window);
		}
	}

	void setLetters(std::string lab) {
		letters.clear();
		label = lab;
		for (char c : label) {
			auto& label(_manager->addEntity());
			SDL_Rect charRect = getLetterRect(c);
			label.addComponent<TransformComponent>(transform->getPosition().x, transform->getPosition().y,
				charRect.w, charRect.h,//!set the dest.w/h from the table and then also set src.x/y/w/h. dest.x/y is based on previous letter and original label position
				1);
			label.addComponent<SpriteComponent>(fontFamily);
			label.GetComponent<SpriteComponent>().srcRect.x = charRect.x;
			label.GetComponent<SpriteComponent>().srcRect.y = charRect.y;

			letters.push_back(&label);

		}
	}
};


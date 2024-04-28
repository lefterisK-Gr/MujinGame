#pragma once

#include "../PNG_Letters.h"
#include "Components.h"

extern Manager manager;

class UILabel : public Component
{
private:
	std::vector<Entity*> letters;
	std::string label;
	std::string fontFamily;
	bool isHud = false;
public:
	TransformComponent* transform = nullptr;

	UILabel() = default;
	UILabel(std::string lab, std::string fontFam, bool is_hud)
	{
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
				l->GetComponent<TransformComponent>().position.x = Game::camera.x + transform->position.x + previousCharX;
				previousCharX += l->GetComponent<TransformComponent>().width;
			}
		}
	}

	void draw(SpriteBatch& batch) override {
		//draw each letter
		for (auto& l : letters) {
			l->draw(batch);
		}
	}

	void setLetters(std::string lab) {
		letters.clear();
		label = lab;
		for (char c : label) {
			auto& label(manager.addEntity());
			SDL_Rect charRect = getLetterRect(c);
			label.addComponent<TransformComponent>(transform->position.x, transform->position.y,
				charRect.w, charRect.h,//!set the dest.w/h from the table and then also set src.x/y/w/h. dest.x/y is based on previous letter and original label position
				1);
			label.addComponent<SpriteComponent>(fontFamily);
			label.GetComponent<SpriteComponent>().srcRect.x = charRect.x;
			label.GetComponent<SpriteComponent>().srcRect.y = charRect.y;

			letters.push_back(&label);

		}
	}
};


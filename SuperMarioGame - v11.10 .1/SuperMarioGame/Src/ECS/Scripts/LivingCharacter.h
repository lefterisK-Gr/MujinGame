#pragma once

#include "../Components.h"

class LivingCharacter : public Component
{
	int _healthPoints = 50;
public:
	

	LivingCharacter()
	{

	}

	~LivingCharacter() {

	}

	void init() override {

	}

	void update(float deltaTime) override {

	}

	bool applyDamage(float damage) {

		_healthPoints -= damage;
		// If we died, return true
		if (_healthPoints <= 0) {
			return true;
		}
		return false;
	}

};
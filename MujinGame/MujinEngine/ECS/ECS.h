#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

#include <SDL/SDL.h>
#include "../SpriteBatch/SpriteBatch.h"
class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID(); // typeID is unique for each function type T and only initialized once.
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init(){}
	virtual void update(float deltaTime) {}
	virtual void draw(SpriteBatch& batch) {}

	virtual SDL_Rect getRect() 
	{
		SDL_Rect a;
		a.x = a.y = a.w = a.h = 0;
		return a;
	}

	virtual bool getHasGridAbove()
	{
		return false;
	}

	virtual ~Component() {}
};

class Entity
{
private:
	Manager& manager;
	bool active = true;
	
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;
public:
	std::vector<std::unique_ptr<Component>> components;
	Entity(Manager& mManager) : manager(mManager) {}
	void update(float deltaTime)
	{
		for (auto& c : components) c->update(deltaTime);
	}
	void draw(SpriteBatch& batch) 
	{
		for (auto& c : components) c->draw(batch);
	}
	bool isActive() { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}
	//! have addScript function
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void update( float deltaTime = 1.0f)
	{
		for (auto& e : entities) e->update(deltaTime);
	}
	void draw(SpriteBatch& batch)
	{
		for (auto& e : entities) e->draw(batch);
	}
	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
				std::end(v));
		}


		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
				std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

	void clearAllEntities() {
		for (auto& group : groupedEntities) {
			group.clear();
		}
		entities.clear();
	}
};
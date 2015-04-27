#pragma once

#include "object/Object.hpp"


class Entity : public Object
{
public:
	Entity();
	virtual ~Entity();

	sf::Vector2f getPosition() const;

	bool isEntity() const;
	virtual bool isPlayerEntity() const = 0;
	bool isGameObject() const;
	bool isLight() const;

protected:
	sf::Vector2f m_velocity;
};

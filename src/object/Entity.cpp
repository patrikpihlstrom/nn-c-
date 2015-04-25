#include "Entity.hpp"


Entity::Entity() :
	Object()
{
}

Entity::~Entity()
{
}

sf::Vector2f Entity::getPosition() const
{
	return m_polygon.getPosition();
}

bool Entity::isEntity() const
{
	return true;
}

bool Entity::isGameObject() const
{
	return false;
}

bool Entity::isLight() const
{
	return false;
}


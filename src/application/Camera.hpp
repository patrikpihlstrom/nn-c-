#pragma once

#include <SFML/Graphics/View.hpp>

#include "object/Entity.hpp"


class Camera : public sf::View
{
public:
	Camera(const sf::Vector2f& center, const sf::Vector2f& dimensions);
	Camera();
	~Camera();

	void trackEntity(const std::weak_ptr<Entity> entity);
	void update();

private:
	std::weak_ptr<Entity> m_entity;
	sf::Vector2f m_velocity;
};


#include "Camera.hpp"

Camera::Camera() :
	sf::View()
{
}

Camera::Camera(const sf::Vector2f& center, const sf::Vector2f& dimensions) :
	sf::View(center, dimensions)
{
}

Camera::~Camera()
{
}

void Camera::trackEntity(const std::weak_ptr<Entity> entity)
{
	m_entity = entity;
}

void Camera::update()
{
	if (auto entity = m_entity.lock())
	{
		if (getCenter() != entity->getPosition())
		{
			m_velocity.x = (entity->getPosition().x - getCenter().x)/11;
			m_velocity.y = (entity->getPosition().y - getCenter().y)/20;
		}

		move(m_velocity);
	}
}


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

void Camera::update()
{
	/*if (auto entity = m_entity.lock())
	{
		if (getCenter() != entity->getPosition())
		{
			m_velocity.x = (entity->getPosition().x - getCenter().x)/11;
			m_velocity.y = (entity->getPosition().y - getCenter().y)/20;

			if (std::abs(m_velocity.x) < 0.01)
			{
				move(entity->getPosition().x - getCenter().x, 0);
				m_velocity.x = 0;
			}

			if (std::abs(m_velocity.y) < 0.01)
			{
				move(0, entity->getPosition().y - getCenter().y);
				m_velocity.y = 0;
			}
		}

		move(m_velocity);
	}
	*/
}

void Camera::zoom(const float&factor)
{
	sf::View::zoom(factor);
	m_zoom += factor;
}

float Camera::getZoom() const
{
	return m_zoom;
}


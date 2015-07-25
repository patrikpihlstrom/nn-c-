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

void Camera::update(const sf::Vector2f& actorPosition, const sf::Vector2<unsigned char>& actorBounds)
{
	if (getCenter() != actorPosition)
	{
		m_velocity.x = (actorPosition.x + actorBounds.x/2 - getCenter().x)/11;
		m_velocity.y = (actorPosition.y + actorBounds.y/2 - getCenter().y)/11;

		if (std::abs(m_velocity.x) < 0.01)
		{
			move(actorPosition.x + actorBounds.x/2 - getCenter().x, 0);
			m_velocity.x = 0;
		}

		if (std::abs(m_velocity.y) < 0.01)
		{
			move(0, actorPosition.y + actorBounds.y/2 - getCenter().y);
			m_velocity.y = 0;
		}
	}

	move(m_velocity);
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


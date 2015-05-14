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

void Camera::trackActor(const std::weak_ptr<Actor> actor)
{
	m_actor = actor;
}

void Camera::update()
{
	if (auto actor = m_actor.lock())
	{
		if (getCenter() != actor->getPosition())
		{
			m_velocity.x = (actor->getPosition().x + actor->getBounds().width/2 - getCenter().x)/11;
			m_velocity.y = (actor->getPosition().y + actor->getBounds().height/2 - getCenter().y)/11;

			if (std::abs(m_velocity.x) < 0.01)
			{
				move(actor->getPosition().x + actor->getBounds().width/2 - getCenter().x, 0);
				m_velocity.x = 0;
			}

			if (std::abs(m_velocity.y) < 0.01)
			{
				move(0, actor->getPosition().y + actor->getBounds().height/2 - getCenter().y);
				m_velocity.y = 0;
			}
		}

		move(m_velocity);
	}
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


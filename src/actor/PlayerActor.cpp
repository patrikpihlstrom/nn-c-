#include "PlayerActor.hpp"

PlayerActor::PlayerActor() :
	Actor()
{
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::control(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (m_velocity.y > -6)
		{
			if (m_velocity.y > 0)
				m_velocity.y += -m_velocity.y*deltaTime*4;

			m_velocity.y -= 6*deltaTime*2;
		}
		else
			m_velocity.y = -6;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (m_velocity.y < 6)
		{
			if (m_velocity.y < 0)
				m_velocity.y += -m_velocity.y*deltaTime*4;

			m_velocity.y += 6*deltaTime*2;
		}
		else
			m_velocity.y = 6;
	}
	else
	{
		if (std::abs(m_velocity.y) > 0.5)
			m_velocity.y += -m_velocity.y*deltaTime*4;
		else
			m_velocity.y = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (m_velocity.x > -6)
		{
			if (m_velocity.x > 0)
				m_velocity.x += -m_velocity.x*deltaTime*4;

			m_velocity.x -= 6*deltaTime*2;
		}
		else
			m_velocity.x = -6;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_velocity.x < 6)
		{
			if (m_velocity.x < 0 )
				m_velocity.x += -m_velocity.x*deltaTime*4;

			m_velocity.x += 6*deltaTime*2;
		}
		else
			m_velocity.x = 6;
	}
	else
	{
		if (std::abs(m_velocity.x) > 0.5)
			m_velocity.x += -m_velocity.x*deltaTime*4;
		else
			m_velocity.x = 0;
	}
}

bool PlayerActor::isPlayer() const
{
	return true;
}

bool PlayerActor::isNPC() const
{
	return false;
}


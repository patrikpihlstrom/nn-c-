#include "PlayerActor.hpp"

PlayerActor::PlayerActor() :
	Actor()
{
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::control()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (m_velocity.y > -300)
			m_velocity.y -= 20;
		else
			m_velocity.y = -300;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (m_velocity.y < 300)
			m_velocity.y += 20;
		else
			m_velocity.y = 300;
	}
	else
	{
		if (std::abs(m_velocity.y) > 50)
			m_velocity.y += -m_velocity.y/10;
		else
			m_velocity.y = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (m_velocity.x > -300)
			m_velocity.x -= 20;
		else
			m_velocity.x = -300;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_velocity.x < 300)
			m_velocity.x += 20;
		else
			m_velocity.x = 300;
	}
	else
	{
		if (std::abs(m_velocity.x) > 50)
			m_velocity.x += -m_velocity.x/10;
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


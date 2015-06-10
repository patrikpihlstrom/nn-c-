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
		if (m_velocity.y > -TERMINAL_Y_WALK)
			m_velocity.y -= 10;
		else
			m_velocity.y = -TERMINAL_Y_WALK;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (m_velocity.y < TERMINAL_Y_WALK)
			m_velocity.y += 10;
		else
			m_velocity.y = TERMINAL_Y_WALK;
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
		if (m_velocity.x > -TERMINAL_X_WALK)
			m_velocity.x -= 10;
		else
			m_velocity.x = -TERMINAL_X_WALK;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_velocity.x < TERMINAL_X_WALK)
			m_velocity.x += 10;
		else
			m_velocity.x = TERMINAL_X_WALK;
	}
	else
	{
		if (std::abs(m_velocity.x) > 50)
			m_velocity.x += -m_velocity.x/10;
		else
			m_velocity.x = 0;
	}

	if (math::magnitude<float>(m_velocity) > TERMINAL_X_WALK)
	{
		math::normalize(m_velocity);
		m_velocity.x *= TERMINAL_X_WALK;
		m_velocity.y *= TERMINAL_X_WALK;
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


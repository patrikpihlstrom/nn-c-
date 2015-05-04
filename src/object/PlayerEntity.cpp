#include "PlayerEntity.hpp"


PlayerEntity::PlayerEntity()
{
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::update(const float& deltaTime)
{
	control(deltaTime);
	move(m_velocity.x, m_velocity.y);
}

void PlayerEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

bool PlayerEntity::isPlayerEntity() const
{
	return true;
}

void PlayerEntity::control(const float& deltaTime)
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


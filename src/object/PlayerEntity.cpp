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
	m_velocity.x = 0;
	m_velocity.y = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_velocity.y = -10;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_velocity.y = 10;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_velocity.x = -10;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_velocity.x = 10;
}


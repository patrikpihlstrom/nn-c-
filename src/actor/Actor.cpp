#include "Actor.hpp"

Actor::Actor() : 
	m_health(100)
{
}

Actor::~Actor()
{
}

void Actor::update(const float& deltaTime)
{
	control(deltaTime);
	updatePosition();
}

void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

uint8_t Actor::getHealth() const
{
	return m_health;
}

void Actor::setHealth(const uint8_t& health)
{
	m_health = health;
}

void Actor::damage(const int8_t& factor)
{
	if (m_health - factor >= 0)
		m_health -= factor;
	else
		m_health = 0;
}

void Actor::setTexture(const std::weak_ptr<sf::Texture> texture)
{
	if (auto _texture = texture.lock())
		m_shape.setTexture(_texture.get());
}

void Actor::control(const float& deltaTime)
{
}

void Actor::updatePosition()
{
	m_bounds.left += m_velocity.x;
	m_bounds.top += m_velocity.y;
	m_shape.move(m_velocity.x, m_velocity.y);
	move(m_velocity.x, m_velocity.y);
}

ActorId Actor::getId() const
{
	return m_id;
}

void Actor::assign(const ActorId& id)
{
	m_id = id;
}

sf::Rect<int> Actor::getBounds() const
{
	return m_bounds;
}

void Actor::setBounds(const sf::Rect<int>& bounds)
{
	m_bounds = bounds;
}

sf::ConvexShape Actor::getShape() const
{
	return m_shape;
}

void Actor::setShape(const sf::ConvexShape& shape)
{
	m_shape = shape;
}


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
	control();
	updatePosition(deltaTime);
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

void Actor::control()
{
}

void Actor::updatePosition(const float& deltaTime)
{
	move(m_velocity.x*deltaTime, m_velocity.y*deltaTime);
}

void Actor::move(const float x, const float y)
{
	m_shape.move(x, y);
	this->Transformable::move(x, y);
	m_bounds.left = getPosition().x;
	m_bounds.top = getPosition().y;
}

void Actor::setSize(const uint8_t size)
{
	m_size = size;

	m_shape.setPointCount(4);
	m_shape.setPoint(0, getPosition());
	m_shape.setPoint(1, sf::Vector2f(getPosition().x + m_size, getPosition().y));
	m_shape.setPoint(2, sf::Vector2f(getPosition().x + m_size, getPosition().y + m_size));
	m_shape.setPoint(3, sf::Vector2f(getPosition().x, getPosition().y + m_size));

	m_shape.setFillColor(sf::Color(255, 0, 0));

	m_bounds.left = getPosition().x;
	m_bounds.top = getPosition().y;
	m_bounds.width = size;
	m_bounds.height = size;
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

void Actor::setPositionMaster(const float x, const float y)
{
	m_shape.setPosition(x, y);
	this->Transformable::setPosition(x, y);
	m_bounds.left = getPosition().x;
	m_bounds.top = getPosition().y;
}

void Actor::setVelocity(const float x, const float y)
{
	m_velocity = {x, y};
}

float Actor::getPositionX() const
{
	return getPosition().x;
}

float Actor::getPositionY() const
{
	return getPosition().y;
}

float Actor::getVelocityX() const
{
	return m_velocity.x;
}

float Actor::getVelocityY() const
{
	return m_velocity.y;
}


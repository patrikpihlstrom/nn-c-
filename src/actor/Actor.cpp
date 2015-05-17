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
	m_polygon.offset(x, y);
	this->Transformable::move(x, y);
	m_bounds.left = getPosition().x;
	m_bounds.top = getPosition().y;
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

math::Polygon Actor::getPolygon() const
{
	return m_polygon;
}

void Actor::setPolygon(const math::Polygon& polygon)
{
	m_polygon = polygon;
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
	m_polygon.offset(x - getPosition().x, y - getPosition().y);
	move(x - getPosition().x, y - getPosition().y);
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


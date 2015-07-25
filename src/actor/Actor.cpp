#include "Actor.hpp"

Actor::Actor() 
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

void Actor::setHealth(const uint8_t health)
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
}

void Actor::setSize(const uint8_t width, const uint8_t height)
{
	m_size = {width, height};

	m_shape.setPointCount(4);
	m_shape.setPoint(0, getPosition());
	m_shape.setPoint(1, sf::Vector2f(getPosition().x + width, getPosition().y));
	m_shape.setPoint(2, sf::Vector2f(getPosition().x + width, getPosition().y + height));
	m_shape.setPoint(3, sf::Vector2f(getPosition().x, getPosition().y + height));
}

ActorId Actor::getId() const
{
	return m_id;
}

void Actor::assign(const ActorId& id)
{
	m_id = id;
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
	setPosition(x, y);
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

bool Actor::operator<(const Actor& actor) const
{
	return getPosition().y + m_size.y < actor.getPosition().y + actor.getSize().y;
}

sf::Vector2<uint8_t> Actor::getSize() const
{
	return m_size;
}

sf::Rect<int> Actor::getBounds() const
{
	return {(int)(getPosition().x - m_size.x/2), (int)(getPosition().y - m_size.y/2), m_size.x, m_size.y};
}


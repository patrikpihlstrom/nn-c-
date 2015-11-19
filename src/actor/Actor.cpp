#include "Actor.hpp"

Actor::Actor() 
{
	m_facingRight = true;
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
	target.draw(m_sprite, states);
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
		m_sprite.setTexture(*_texture.get(), true);

	m_size = {(uint8_t)m_sprite.getTextureRect().width, (uint8_t)m_sprite.getTextureRect().height};
}

void Actor::control()
{
	if ((m_facingRight && m_velocity.x < 0) || (!m_facingRight && m_velocity.x > 0))
	{
		m_facingRight = !m_facingRight;
		m_sprite.scale(-1.f, 1.f);
	}
}

void Actor::updatePosition(const float& deltaTime)
{
	move(m_velocity.x*deltaTime, m_velocity.y*deltaTime);
}

void Actor::move(const float x, const float y)
{
	Transformable::move(x, y);
	m_sprite.setPosition(getPosition());
}

void Actor::setSize(const uint8_t width, const uint8_t height)
{
	m_sprite.scale((float)width/m_size.x, (float)height/m_size.y);
	m_size = {width, height};
	m_sprite.setOrigin(width/2, height/2);
}

ActorId Actor::getId() const
{
	return m_id;
}

void Actor::assign(const ActorId& id)
{
	m_id = id;
}

uint64_t Actor::getIdAsInt() const
{
	return m_id.id;
}

sf::Sprite Actor::getSprite() const
{
	return m_sprite;
}

void Actor::setSprite(const sf::Sprite& sprite)
{
	m_sprite = sprite;
}

void Actor::setPositionMaster(const float x, const float y)
{
	m_sprite.setPosition(x, y);
	Transformable::setPosition(x, y);
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

float Actor::getPlayerPositionX() const
{
	return getPosition().x;
}

float Actor::getPlayerPositionY() const
{
	return getPosition().y;
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

bool Actor::hasPlayer() const
{
	return false;
}


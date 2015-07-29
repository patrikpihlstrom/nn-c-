#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "actor/ActorId.hpp"
#include "application/Math.hpp"
#include "application/lua.hpp"


class Actor : public sf::Transformable, public sf::Drawable
{
public:
	Actor();
	~Actor();

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	uint8_t getHealth() const;
	void setHealth(const uint8_t health);
	void damage(const int8_t& factor);

	void setTexture(const std::weak_ptr<sf::Texture> texture);

	virtual bool isPlayer() const = 0;
	virtual bool isNPC() const = 0;

	ActorId getId() const;
	void assign(const ActorId& id);

	sf::Sprite getSprite() const;
	void setSprite(const sf::Sprite& shape);

	void setPositionMaster(const float x, const float y);
	void setVelocity(const float x, const float y);
	void move(const float x, const float y);
	void setSize(const uint8_t width, const uint8_t height);

	float getPositionX() const;
	float getPositionY() const;
	float getVelocityX() const;
	float getVelocityY() const;
	sf::Vector2<uint8_t> getSize() const;

	bool operator<(const Actor& actor) const;

	sf::Rect<int> getBounds() const;
	
protected:
	virtual void control();
	virtual void updatePosition(const float& deltaTime);

	sf::Vector2f m_velocity;
	sf::Vector2<uint8_t> m_size;

	sf::Sprite m_sprite;

	ActorId m_id;

	uint8_t m_health;

private:
	bool m_facingRight;
};


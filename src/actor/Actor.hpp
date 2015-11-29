#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "actor/ActorId.hpp"
#include "application/Math.hpp"


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
	uint64_t getIdAsInt() const;

	sf::Sprite getSprite() const;
	void setSprite(const sf::Sprite& shape);

	void setOrigin(const int x, const int y);
	void setPositionMaster(const float x, const float y);
	void setVelocity(const float x, const float y);
	void move(const float x, const float y);
	void setSize(const uint8_t width, const uint8_t height);

	float getOriginX() const;
	float getOriginY() const;
	float getPositionX() const;
	float getPositionY() const;
	float getVelocityX() const;
	float getVelocityY() const;

	virtual float getPlayerPositionX() const; // Returns getPosition().x if is player
	virtual float getPlayerPositionY() const; // Returns getPosition().y if is player

	virtual bool hasPlayer() const; // Only defined in NPCActor.

	sf::Vector2<uint8_t> getSize() const;

	bool operator<(const Actor& actor) const;

	sf::Rect<int> getBounds() const;
	
	virtual void control();
	virtual void updatePosition(const float& deltaTime);
protected:

	sf::Vector2f m_velocity;
	sf::Vector2<uint8_t> m_size;
	sf::Vector2i m_origin;

	sf::Sprite m_sprite;

	ActorId m_id;

	uint8_t m_health;

private:
	bool m_facingRight;
};


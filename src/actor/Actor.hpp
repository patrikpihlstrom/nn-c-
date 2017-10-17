#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "ActorId.hpp"
#include "../application/Math.hpp"


class Actor : public sf::Transformable, public sf::Drawable
{
public:
	const float MAX_SPEED = 50;
	const float MAX_ACC = 0.01f;
	const float MAX_ROTATION_RATE = 0.01f;
	const float MAX_ROTATION_ACC = 0.0005f;
	
	Actor();
	~Actor();

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	uint8_t getHealth() const;
	void setHealth(const uint8_t health);
	void damage(const int8_t& factor);

	ActorId getId() const;
	void assign(const ActorId& id);
	uint64_t getIdAsInt() const;

	void move(const float x, const float y);

	bool operator<(const Actor& actor) const;
	
	virtual void control();
	virtual void updatePosition(const float& deltaTime);

	float getSize() const;
	void setSize(const float size);

	virtual sf::Rect<int> getBounds() const;

protected:
	float m_desiredSpeed;
	float m_desiredRotationRate;
	float m_rotationRate;
	float m_angle;
	float m_size;

	ActorId m_id;

	bool m_dead;
	uint8_t m_health;

	sf::Vector2f m_velocity;

private:
};


#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "actor/ActorId.hpp"


class Actor : public sf::Transformable, public sf::Drawable
{
public:
	Actor();
	~Actor();

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	uint8_t getHealth() const;
	void setHealth(const uint8_t& health);
	void damage(const int8_t& factor);

	void setTexture(const std::weak_ptr<sf::Texture> texture);

	virtual bool isPlayer() const = 0;
	virtual bool isNPC() const = 0;

	ActorId getId() const;
	void assign(const ActorId& id);

	sf::Rect<int> getBounds() const;
	void setBounds(const sf::Rect<int>& bounds);

	sf::ConvexShape getShape() const;
	void setShape(const sf::ConvexShape& shape);
	
protected:
	virtual void control(const float& deltaTime);
	virtual void updatePosition();

	sf::Vector2f m_velocity;
	sf::Rect<int> m_bounds;
	uint8_t m_health;

	sf::ConvexShape m_shape;

	ActorId m_id;
};


#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "ObjectIdTracker.hpp"
#include "../application/Math.hpp"


class Object : public sf::Drawable, public sf::Transformable
{
public:
	Object();
	Object(const ObjectId& id);
	virtual ~Object();

	void assign(const ObjectId& id);
	ObjectId getId() const;

	math::Polygon getPolygon() const;
	void setPolygon(const math::Polygon& polygon);

	void updateShape();
	virtual void setTexture(const std::weak_ptr<sf::Texture> texture);
	void setTextureRect(const sf::Rect<int>& rect);

	sf::FloatRect getGlobalBounds() const;
	sf::FloatRect getLocalBounds() const;

	void move(const float x, const float y);

	virtual void update(const float& deltaTime);
	virtual void drawShadow(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool isEntity() const = 0;
	virtual bool isPlayerEntity() const = 0;
	virtual bool isGameObject() const = 0;
	virtual bool isLight() const = 0;

protected:
	ObjectId m_id;
	math::Polygon m_polygon;
	sf::Vector2f m_position;

	sf::ConvexShape m_shape;

	virtual void control(const float& deltaTime);
};


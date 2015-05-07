#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

#include "object/ObjectIdTracker.hpp"
#include "application/Math.hpp"


class Object : public sf::Drawable, public sf::Transformable
{
public:
	Object();
	Object(const ObjectId& id);
	virtual ~Object();

	void assign(const ObjectId& id);
	ObjectId getId() const;

	sf::Rect<int> getBoundingBox() const;

	void updateShape();
	void setPolygon(const math::Polygon& polygon);
	math::Polygon getPolygon() const;

	virtual void setTexture(const std::weak_ptr<sf::Texture> texture);
	void setTextureRect(const sf::Rect<int>& rect);

	void move(const float x, const float y);

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool isGameObject() const = 0;
	virtual bool isLight() const = 0;

protected:
	ObjectId m_id;

	math::Polygon m_polygon;
	sf::Rect<int> m_boundingBox;

	sf::ConvexShape m_shape;

	virtual void control(const float& deltaTime);
};


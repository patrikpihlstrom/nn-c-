#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

#include "../application/Math.hpp"
#include "ObjectId.hpp"

enum class ObjectType
{
	obstacle,
	food,
	start,
	finish
};

class Object : public sf::Drawable, public sf::Transformable
{
public:
	Object();
	Object(const ObjectId id);
	virtual ~Object();

	void assign(const ObjectId id);
	ObjectId getId() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f getSize() const;
	void setSize(const sf::Vector2f size);

	sf::Rect<float> getBoundingBox() const;
	sf::Rect<int> getBoundingBoxInt() const;

	math::Polygon getPolygon() const;
	void setPolygon(const math::Polygon& polygon);

	bool hasPolygon() const;

	ObjectType getType() const;
	void setType(const ObjectType type);

	sf::Color getColor() const;

	bool dead = false;

protected:
	ObjectId m_id;
	ObjectType m_type;
	sf::Vector2f m_size;
	math::Polygon m_polygon;
	sf::ConvexShape m_shape;
	sf::Color m_color;

};


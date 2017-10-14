#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

#include "application/Math.hpp"


class Object : public sf::Drawable, public sf::Transformable
{
public:
	Object();
	virtual ~Object();

	sf::Rect<int> getBoundingBox() const;

	void updateShape();
	void setPolygon(const math::Polygon& polygon);
	math::Polygon getPolygon() const;

	virtual void setTexture(const std::weak_ptr<sf::Texture> texture);
	void setTextureRect(const sf::Rect<int>& rect);

	void move(const float x, const float y);

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	math::Polygon m_polygon;
	sf::Rect<int> m_boundingBox;

	sf::ConvexShape m_shape;

	virtual void control(const float& deltaTime);
};


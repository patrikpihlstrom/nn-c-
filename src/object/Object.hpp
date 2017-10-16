#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

#include "../application/Math.hpp"
#include "ObjectId.hpp"


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

	sf::Rect<int> getBoundingBox() const;

protected:
	ObjectId m_id;
	sf::Vector2f m_size;

};


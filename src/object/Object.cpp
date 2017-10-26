#include "Object.hpp"


Object::Object()
{
}

Object::Object(const ObjectId id) :
	m_id(id)
{
}

Object::~Object()
{
}

void Object::assign(const ObjectId id)
{
	m_id = id;
}

ObjectId Object::getId() const
{
	return m_id;
}

sf::Vector2f Object::getSize() const
{
	return m_size;
}

void Object::setSize(const sf::Vector2f size)
{
	m_size = size;
}

sf::Rect<float> Object::getBoundingBox() const
	{if (m_polygon.getPointCount())
	{
		auto bounds = m_polygon.getBounds();
		return {(float)bounds.left, (float)bounds.top, (float)bounds.width, (float)bounds.height};
	}

	return {getPosition().x, getPosition().y, m_size.x, m_size.y};
}

sf::Rect<int> Object::getBoundingBoxInt() const
{
	if (m_polygon.getPointCount())
	{
		return m_polygon.getBounds();
	}

	return {(int)getPosition().x, (int)getPosition().y, (int)m_size.x, (int)m_size.y};
}

math::Polygon Object::getPolygon() const
{
	return m_polygon;
}

void Object::setPolygon(const math::Polygon& polygon)
{
	m_polygon = polygon;
	m_shape.setPointCount(polygon.getPointCount());
	for (int i = 0; i < m_shape.getPointCount(); ++i)
	{
		m_shape.setPoint(i, polygon.getPoint(i));
	}
	m_shape.setFillColor({100, 255, 100});
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_shape.getPointCount() > 0)
	{
		target.draw(m_shape, states);
	}
	else
	{
		sf::RectangleShape shape;
		shape.setPosition(getPosition());
		shape.setSize(m_size);
		shape.setFillColor(sf::Color(100, 100, 100));
		target.draw(shape, states);
	}
}

bool Object::hasPolygon() const
{
	return m_polygon.getPointCount() > 0;
}


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
{
	if (m_polygon.getPointCount() > 0)
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
	m_shape.setFillColor({200, 200, 200});
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	switch (m_type)
	{
		case ObjectType::obstacle:
			if (m_shape.getPointCount() > 0)
			{
				target.draw(m_shape, states);
			}
			else
			{
				sf::RectangleShape shape;
				shape.setPosition(getPosition());
				shape.setSize(m_size);
				shape.setFillColor(m_color);
				target.draw(shape, states);
			}
		break;

		case ObjectType::food:
			sf::CircleShape shape;
			shape.setRadius(12.f);
			shape.setFillColor(m_color);
			shape.setPosition(getPosition());
			target.draw(shape, states);
		break;
	}
	
}

bool Object::hasPolygon() const
{
	return m_polygon.getPointCount() > 0;
}

ObjectType Object::getType() const
{
	return m_type;
}

void Object::setType(const ObjectType type)
{
	m_type = type;
	
	switch (type)
	{
		case ObjectType::food:
			m_size = {24, 24};
			m_color = {120, 100, 250};
		break;

		case ObjectType::obstacle:
			m_color = {100, 100, 100};
		break;
	}
}

sf::Color Object::getColor() const
{
	return m_color;
}


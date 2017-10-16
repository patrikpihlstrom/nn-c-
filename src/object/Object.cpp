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

sf::Rect<int> Object::getBoundingBox() const
{
	return {(int)getPosition().x, (int)getPosition().y, (int)m_size.x, (int)m_size.y};
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape shape;
	shape.setPosition(getPosition());
	shape.setSize(m_size);
	shape.setFillColor(sf::Color(100, 100, 100));
	target.draw(shape, states);
}


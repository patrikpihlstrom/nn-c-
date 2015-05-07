#include "Object.hpp"


Object::Object() :
	m_solid(false)
{
}

Object::Object(const ObjectId& id) :
	m_id(id),
	m_solid(false)
{
}

Object::~Object()
{
}

void Object::assign(const ObjectId& id)
{
	m_id = id;
}

ObjectId Object::getId() const
{
	return m_id;
}

math::Polygon Object::getPolygon() const
{
	return m_polygon;
}

sf::Rect<int> Object::getBoundingBox() const
{
	return m_boundingBox;
}

void Object::setPolygon(const math::Polygon& polygon)
{
	m_polygon = polygon;
	updateShape();

	if (!isLight())
		m_boundingBox = (sf::Rect<int>)m_polygon.getBounds();
}

void Object::updateShape()
{
	m_shape.setFillColor(sf::Color(100, 100, 100));
	m_shape.setPointCount(m_polygon.getPointCount());

	for (int i = 0; i < m_polygon.getPointCount(); ++i)
		m_shape.setPoint(i, m_polygon.getPoint(i));
}

void Object::setTexture(const std::weak_ptr<sf::Texture> texture)
{
	if (auto _texture = texture.lock())
		m_shape.setTexture(_texture.get(), false);
}

void Object::setTextureRect(const sf::Rect<int>& rect)
{
	m_shape.setTextureRect(rect);
}

sf::FloatRect Object::getGlobalBounds() const
{
	return m_shape.getGlobalBounds();
}

sf::FloatRect Object::getLocalBounds() const
{
	return m_shape.getLocalBounds();
}

void Object::update(const float& deltaTime)
{
}

void Object::control(const float& deltaTime)
{
}

void Object::move(const float x, const float y)
{
	m_boundingBox.left += x;
	m_boundingBox.top += y;

	m_position.x += x;
	m_position.y += y;

	m_polygon.offset(x, y);

	m_shape.move(x, y);

	setPosition(m_position);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

void Object::setSolid(const bool solid)
{
	m_solid = solid;
}

bool Object::getSolid() const
{
	return m_solid;
}

void Object::generateLightObstructer()
{
	LightObstructor lightObstructor;
	lightObstructor.setPolygon(m_polygon);

	m_lightObstructor.reset(new LightObstructor(lightObstructor));
}

void Object::addLight(std::weak_ptr<Light> light)
{
	if (m_lightObstructor)
		m_lightObstructor->addLight(light);
}

void Object::removeLight(const ObjectId& id)
{
	if (m_lightObstructor)
		m_lightObstructor->removeLight(id);
}

void Object::clearLights()
{
	if (m_lightObstructor)
		m_lightObstructor->clearLights();
}


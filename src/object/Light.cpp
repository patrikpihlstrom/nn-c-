#include "Light.hpp"


Light::Light() :
	Object()
{
	m_radius = 1024;
	
	m_polygon.clear();
	m_vertexArray.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
}

Light::~Light()
{
}

float Light::getRadius() const
{
	return m_radius;
}

void Light::setRadius(const float& radius)
{
	m_radius = radius;
}

void Light::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.blendMode = sf::BlendAdd;
	if (auto texture = m_texture.lock())
		states.texture = texture.get();

	target.draw(m_vertexArray, states);
}

void Light::clear()
{
	m_vertexArray.clear();
	m_angles.clear();
	m_points.clear();
	m_objects.clear();

	m_angles.push_back(3*M_PI/4);
	m_angles.push_back(M_PI/4);
	m_angles.push_back(-3*M_PI/4);
	m_angles.push_back(-M_PI/4);

	m_angles.push_back(M_PI/2);
	m_angles.push_back(0);
	m_angles.push_back(-M_PI/2);
	m_angles.push_back(-M_PI);

	math::Segment<float> edge{{getPosition().x - m_radius, getPosition().y - m_radius}, {getPosition().x + m_radius, getPosition().y - m_radius}};
	m_objects.push_back(edge);
	
	edge.a = sf::Vector2f(getPosition().x + m_radius, getPosition().y - m_radius);
	edge.b = sf::Vector2f(getPosition().x + m_radius, getPosition().y + m_radius);
	m_objects.push_back(edge);

	edge.a = sf::Vector2f(getPosition().x + m_radius, getPosition().y + m_radius);
	edge.b = sf::Vector2f(getPosition().x - m_radius, getPosition().y + m_radius);
	m_objects.push_back(edge);

	edge.a = sf::Vector2f(getPosition().x - m_radius, getPosition().y + m_radius);
	edge.b = sf::Vector2f(getPosition().x - m_radius, getPosition().y - m_radius);
	m_objects.push_back(edge);
}

void Light::accountForObject(const math::Polygon& polygon)
{
	m_angles.reserve(polygon.getPointCount()*3);
	m_objects.reserve(polygon.getEdgeCount());

	for (int i = 0; i < polygon.getPointCount(); ++i)
	{
		float angle = std::atan2(polygon.getPoint(i).y - getPosition().y, polygon.getPoint(i).x - getPosition().x);
		m_angles.push_back(angle);

		math::Segment<float> segment;
		segment.a = polygon.getPoint(i);

		if (i == polygon.getPointCount() - 1)
			segment.b = polygon.getPoint(0);
		else
			segment.b = polygon.getPoint(i + 1);

		m_objects.push_back(segment);
	}
}

sf::Vector2f Light::castRay(const float& angle)
{
	sf::Vector2f ray{std::cos(angle)*m_radius*(m_points.size() < 8 ? 2:1) + getPosition().x, std::sin(angle)*m_radius*(m_points.size() < 8 ? 2:1) + getPosition().y}, intersection;

	for (int i = 0; i < m_objects.size(); ++i)
	{
		if (math::lineIntersectsLine(getPosition(), ray, m_objects[i].a, m_objects[i].b, intersection))
			ray = intersection;
	}

	return ray;
}

void Light::buildLightShape()
{
	std::sort(m_angles.begin(), m_angles.end());
	m_points.reserve(m_angles.size()*3);

	for (int i = 0; i < m_angles.size(); ++i)
	{
		m_points.push_back(castRay(m_angles[i] - 0.000001));
		m_points.push_back(castRay(m_angles[i] + 0.000001));
	}

	m_vertexArray.append(sf::Vertex(getPosition(), m_color));
	m_vertexArray[0].texCoords = {(float)m_texture.lock()->getSize().x/2, (float)m_texture.lock()->getSize().x/2};
	for (int i = 0; i < m_points.size(); ++i)
	{
		m_vertexArray.append(sf::Vertex(m_points[i], m_color));
		m_vertexArray[i + 1].texCoords = {m_points[i].x - getPosition().x + (float)m_texture.lock()->getSize().x/2, m_points[i].y - getPosition().y + (float)m_texture.lock()->getSize().x/2};
	}
	m_vertexArray.append(sf::Vertex(m_points[0], m_color));
	m_vertexArray[m_vertexArray.getVertexCount() - 1].texCoords = {m_points[0].x - getPosition().x + (float)m_texture.lock()->getSize().x/2, m_points[0].y - getPosition().y + (float)m_texture.lock()->getSize().x/2};
}

void Light::setTexture(const std::weak_ptr<sf::Texture> texture)
{
	m_texture = texture;
}

void Light::setColor(const sf::Color& color)
{
	m_color = color;
}

bool Light::isEntity() const
{
	return false;
}

bool Light::isPlayerEntity() const
{
	return false;
}

bool Light::isGameObject() const
{
	return false;
}

bool Light::isLight() const
{
	return true;
}


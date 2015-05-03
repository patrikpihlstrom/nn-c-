#include "Light.hpp"


Light::Light() :
	Object()
{
	m_radius = 1024;
	
	m_polygon.clear();
	m_vertexArray.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
	m_color = sf::Color(255, 0, 255);

	m_boundingBox.left = getPosition().x - m_radius/2.5f;
	m_boundingBox.top = getPosition().y - m_radius/2.5f;
	m_boundingBox.width = m_radius/1.25f;
	m_boundingBox.height = m_radius/1.25f;
}

Light::~Light()
{
}

void Light::update()
{
	m_boundingBox.left = m_position.x - m_radius/2.5f;
	m_boundingBox.top = m_position.y - m_radius/2.5f;
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
	/*sf::RectangleShape rect(sf::Vector2f(getBoundingBox().width, getBoundingBox().height));
	rect.setPosition(sf::Vector2f(getBoundingBox().left, getBoundingBox().top));
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineThickness(1.f);
	rect.setOutlineColor(sf::Color(255, 0, 0));
	target.draw(rect);
	*/

	if (auto texture = m_texture.lock())
			states.texture = texture.get();

	states.blendMode = sf::BlendAdd;
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

	math::Polygon bounds;
	bounds.addPoint(sf::Vector2f(0, 0));
	bounds.addPoint(sf::Vector2f(0, 0));
	bounds.addPoint(sf::Vector2f(0, 0));
	bounds.addPoint(sf::Vector2f(0, 0));
	bounds.constructEdges();
	m_bounds = bounds;
}

void Light::accountForObject(const math::Polygon& polygon)
{
	m_objects.push_back(polygon);

	auto bounds = polygon.getBounds();
	std::vector<short> projections;

	float min = 0, max = 0;
	bool minAssigned = false, maxAssigned = false;
	bool reverse = getPosition().x > bounds.left + bounds.width && getPosition().y >= bounds.top && getPosition().y <= bounds.top + bounds.height; // Hacky

	if (!reverse)
	{
		sf::Vector2f center{(float)bounds.left + bounds.width/2, (float)bounds.top + bounds.height/2};
		sf::Vector2f axis{getPosition().y - center.y, center.x - getPosition().x};
		projections = polygon.projectPivot(axis, getPosition());
	}

	m_angles.reserve(reverse ? polygon.getPointCount():2);

	for (int i = 0; i < polygon.getPointCount(); ++i)
	{
		auto point = polygon.getPoint(i);
		float angle = std::atan2(point.y - getPosition().y, point.x - getPosition().x);

		if (!reverse)
		{
			if (projections[i] < 0)
			{
				if (angle < min || !minAssigned)
				{
					min = angle;
					minAssigned = true;
				}
			}
			else if (projections[i] > 0)
			{
				if (angle > max || !maxAssigned)
				{
					max = angle;
					maxAssigned = true;
				}
			}
		}
		else
			m_angles.push_back(angle);
	}

	if (!reverse)
	{
		m_angles.push_back(min);
		m_angles.push_back(max);
	}
}

sf::Vector2f Light::castRay(const float& angle)
{
	bool obstructed = false;
	sf::Vector2f ray{std::cos(angle)*m_radius + getPosition().x, std::sin(angle)*m_radius + getPosition().y}, intersection;
	auto rayBounds = math::getBoundingBox<int>((sf::Vector2<int>)getPosition(), (sf::Vector2<int>)ray);
	rayBounds.left--;
	rayBounds.top--;
	rayBounds.width += 2;
	rayBounds.height += 2;

	for (int i = 0; i < m_objects.size(); ++i)
	{
		auto objectBounds = m_objects[i].getBounds();
		objectBounds.left--;
		objectBounds.top--;
		objectBounds.width += 2;
		objectBounds.height += 2;

		if (objectBounds.intersects(rayBounds))
		{
			for (int j = 0; j < m_objects[i].getEdgeCount(); ++j)
			{
				auto edge = m_objects[i].getEdgeSegment<float>(j);
				if (math::lineIntersectsLine(getPosition(), ray, edge.a, edge.b, intersection))
				{
					obstructed = true;
					ray = intersection;
					rayBounds = math::getBoundingBox<int>((sf::Vector2<int>)getPosition(), (sf::Vector2<int>)ray);
					rayBounds.left--;
					rayBounds.top--;
					rayBounds.width += 2;
					rayBounds.height += 2;
				}
			}
		}
	}

	if (!obstructed)
	{
		for (int i = 0; i < m_bounds.getEdgeCount(); ++i)
		{
			auto edge = m_bounds.getEdgeSegment<float>(i);
			if (math::lineIntersectsLine(getPosition(), ray, edge.a, edge.b, intersection))
				ray = intersection;
		}
	}

	return ray;
}

void Light::buildLightShape()
{
	std::sort(m_angles.begin(), m_angles.end());
	m_points.reserve(m_angles.size()*2);

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


#include "Entity.hpp"


Entity::Entity()
{
	m_maxHorizontalSpeed = 12.5;
	m_jumpChecks[0].grounded = false;
	m_jumpChecks[1].grounded = false;
	m_jumpChecks[2].grounded = false;
}

Entity::~Entity()
{
}

void Entity::setQuadtree(const std::weak_ptr<Quadtree> quadtree)
{
	m_quadtree = quadtree;
}

void Entity::collide(const math::Polygon& polygon)
{
	auto intersection = math::SAT(m_polygon, polygon, m_velocity);
	if (intersection.willIntersect)
		m_velocity += intersection.minimumTranslationVector;

	for (int i = 0; i < 3; ++i)
	{
		intersection = math::SAT(m_jumpChecks[i].polygon, polygon, m_velocity);
		if (intersection.willIntersect || intersection.intersect)
			m_jumpChecks[i].grounded = true;
	}
}

void Entity::update(const float deltaTime)
{
	m_velocity.y += .75;

	if (m_velocity.y > 20)
		m_velocity.y = 20;

	control(deltaTime);
	checkCollisions();
	move(m_velocity);
}

void Entity::checkCollisions()
{
	for (int i = 0; i < 3; ++i)
		m_jumpChecks[i].grounded = false;

	if (auto quadtree = m_quadtree.lock())
	{
		std::vector<std::weak_ptr<Quadtree>> quadtrees;
		quadtree->getQuadtrees(quadtrees, m_polygon);

		if (!quadtrees.empty())
		{
			std::vector<unsigned int> indices;

			for (int i = 0; i < quadtrees.size(); ++i)
			{
				if (auto _quadtree = quadtrees[i].lock())
				{
					m_velocity = _quadtree->checkCollisions(m_polygon, m_jumpChecks[0], m_jumpChecks[1], m_jumpChecks[2], m_velocity, indices);

					/*auto polygons = _quadtree->getPolygons(indices);

					for (int j = 0; j < polygons.size(); ++j)
					{
						collide(*polygons[j]);
					}*/
				}
			}
		}
	}
}

void Entity::move(const sf::Vector2f& offset)
{
	m_polygon.offset(offset.x, offset.y);

	for (int i = 0; i < 3; ++i)
		m_jumpChecks[i].polygon.offset(offset.x, offset.y);

	m_shape.move(offset.x, offset.y);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

sf::Vector2f Entity::getPosition() const
{
	float x = 0, y = 0;
	for (int i = 0; i < m_polygon.getPointCount(); ++i)	
	{
		x += m_polygon.getPoint(i).x;
		y += m_polygon.getPoint(i).y;
	}

	x /= m_polygon.getPointCount();
	y /= m_polygon.getPointCount();

	return sf::Vector2f(x, y);
}

sf::Vector2f Entity::getVelocity() const
{
	return m_velocity;
}

float Entity::getMaxHorizontalSpeed() const
{
	return m_maxHorizontalSpeed;
}


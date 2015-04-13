#include "Entity.hpp"


Entity::Entity()
{
	m_grounded[0] = false;
	m_grounded[1] = false;
	m_grounded[2] = false;
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
		intersection = math::SAT(m_checks[i], polygon, m_velocity);
		if (intersection.willIntersect || intersection.intersect)
			m_grounded[i] = true;
	}
}

void Entity::update(const float deltaTime)
{
	m_velocity.y += .5;

	if (m_velocity.y > 20)
		m_velocity.y = 20;

	control(deltaTime);

	checkCollisions();
	move(m_velocity);
}

void Entity::checkCollisions()
{
	m_grounded[0] = false;
	m_grounded[1] = false;
	m_grounded[2] = false;

	if (auto quadtree = m_quadtree.lock())
	{
		std::vector<std::weak_ptr<Quadtree>> quadtrees;
		quadtree->getQuadtrees(quadtrees, m_polygon);

		if (!quadtrees.empty())
		{
			std::vector<unsigned char> indices;

			for (int i = 0; i < quadtrees.size(); ++i)
			{
				if (auto _quadtree = quadtrees[i].lock())
				{
					auto polygons = _quadtree->getPolygons(indices);

					for (int j = 0; j < polygons.size(); ++j)
					{
						collide(*polygons[j]);
					}
				}
			}
		}
	}
}

void Entity::move(const sf::Vector2f& offset)
{
	m_polygon.offset(offset.x, offset.y);

	for (int i = 0; i < 3; ++i)
		m_checks[i].offset(offset.x, offset.y);

	m_shape.move(offset.x, offset.y);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}


#include "Entity.hpp"


Entity::Entity()
{
	m_maxHorizontalSpeed = 12.5;
}

Entity::~Entity()
{
}

void Entity::setQuadtree(const std::weak_ptr<Quadtree> quadtree)
{
	m_quadtree = quadtree;
}

void Entity::update(const float deltaTime)
{
	m_acceleration.y = 9.82*4*deltaTime;
	m_velocity.y += m_acceleration.y;

	control();
	terminalVelocity();
	checkCollisions();
	move(m_velocity);
}

void Entity::terminalVelocity()
{
}

void Entity::checkCollisions()
{
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
					m_velocity = _quadtree->checkCollisions(m_polygon, m_velocity, indices);
				}
			}
		}
	}
}

void Entity::move(const sf::Vector2f& offset)
{
	m_polygon.offset(offset.x, offset.y);

	
	m_shape.move(offset.x, offset.y);

	entitySpecificMovement(offset);
}

void Entity::entitySpecificMovement(const sf::Vector2f& offset)
{
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


#include "Quadtree.hpp"

Quadtree::Quadtree() :
	m_hasChildren(false),
	m_level(0)
{
}

Quadtree::Quadtree(const sf::Vector2f& position, const sf::Vector2f& dimensions, const bool hasChildren, const unsigned char level) :
	m_position(position),
	m_dimensions(dimensions),
	m_hasChildren(hasChildren),
	m_level(level)
{
	polygon.addPoint((sf::Vector2f)position);
	polygon.addPoint(sf::Vector2f(position.x + dimensions.x, position.y));
	polygon.addPoint(sf::Vector2f(position.x + dimensions.x, position.y + dimensions.y));
	polygon.addPoint(sf::Vector2f(position.x, position.y + dimensions.y));
}

Quadtree::~Quadtree()
{
}

void Quadtree::update()
{
	if (canMergeChildren())
		mergeChildren();
}

void Quadtree::insert(std::shared_ptr<math::Polygon> polygon, unsigned char index)
{
	if (!m_hasChildren)
	{
		{
			m_polygons[index] = polygon;

			if (m_level < MAX_LEVEL)
				split();
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, *polygon))
				m_children[i]->insert(polygon, index);
	}
}

bool Quadtree::remove(unsigned char index)
{
	if (!m_children[0])
	{
		auto it = m_polygons.find(index);
		if (it != m_polygons.end())
		{
			m_polygons.erase(it);
			return true;
		}
		else
			return false;
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_children[i]->remove(index))
				return true;
		}
	}

	return false;
}

void Quadtree::split()
{
	if (m_hasChildren)
		return;

	m_hasChildren = true;

	m_children[0].reset(new Quadtree(m_position, sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));
	m_children[1].reset(new Quadtree(sf::Vector2f(m_position.x + m_dimensions.x/2, m_position.y), sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));
	m_children[2].reset(new Quadtree(sf::Vector2f(m_position.x, m_position.y + m_dimensions.y/2), sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));
	m_children[3].reset(new Quadtree(sf::Vector2f(m_position.x + m_dimensions.x/2, m_position.y + m_dimensions.y/2), sf::Vector2f(m_dimensions.x/2, m_dimensions.y/2), false, m_level + 1));

	for (int i = 0; i < 4; ++i)
	{
		for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
			if (math::polygonIntersectsPolygon(m_children[i]->polygon, *it->second))
				m_children[i]->insert(it->second, it->first);
	}

	m_polygons.clear();
}

bool Quadtree::canMerge() const
{
	if (auto parent = m_parent.lock())
	{
		for (int i = 0; i < 4; ++i)
		{
			if (!parent->m_children[i]->empty())
				return false;
		}

		return true;
	}

	return false;
}

bool Quadtree::canMergeChildren() const
{
	if (!m_children[0])
		return false;

	for (int i = 0; i < 4; ++i)
	{
		if (!m_children[i]->empty())
			return false;
	}

	return true;
}

bool Quadtree::empty() const
{
	return m_polygons.empty();
}

void Quadtree::mergeChildren()
{
	for (int i = 0; i < 4; ++i)
		m_children[i].reset();

	m_hasChildren = false;
}

std::shared_ptr<math::Polygon> Quadtree::getPolygon(const sf::Vector2f& position)
{
	if (m_children[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < m_children[i]->m_polygons.size(); ++j)
			{
				if (math::pointInPolygon(position, *m_children[i]->m_polygons[j]))
					return m_children[i]->m_polygons[j];
			}
		}
	}

	return nullptr;
}

void Quadtree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
			m_children[i]->draw(target, states);
	}
	else
	{
		sf::ConvexShape shape;
		shape.setFillColor(sf::Color(255, 0, 0, 0));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(255, 0, 0, 100));
		shape.setPointCount(4);

		shape.setPoint(0, m_position);
		shape.setPoint(1, sf::Vector2f(m_position.x + m_dimensions.x, m_position.y));
		shape.setPoint(2, sf::Vector2f(m_position.x + m_dimensions.x, m_position.y + m_dimensions.y));
		shape.setPoint(3, sf::Vector2f(m_position.x, m_position.y + m_dimensions.y));

		target.draw(shape, states);
	}
}

void Quadtree::save(std::ofstream& file, std::vector<unsigned char>& saved) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
			m_children[i]->save(file, saved);
	}
	else
	{
		for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
		{
			if (std::find(saved.begin(), saved.end(), it->first) == saved.end())
			{
				for (int j = 0; j < it->second->getPointCount(); ++j)
					file << it->second->getPoint(j).x << ':' << it->second->getPoint(j).y << '\n';

				file << "-\n";
				saved.push_back(it->first);
			}
		}
	}
}

bool Quadtree::hasChildren() const
{
	return m_hasChildren;
}

void Quadtree::getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const math::Polygon& polygon) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_children[i]->hasChildren())
				m_children[i]->getQuadtrees(quadtrees, polygon);
			else if (math::polygonIntersectsPolygon(polygon, m_children[i]->polygon))
				quadtrees.push_back(m_children[i]);
		}
	}
}

std::vector<std::shared_ptr<math::Polygon>> Quadtree::getPolygons(std::vector<unsigned char>& indices) const
{
	std::vector<std::shared_ptr<math::Polygon>> polygons;
	for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
	{
		if (std::find(indices.begin(), indices.end(), it->first) == indices.end())
		{
			indices.push_back(it->first);
			polygons.push_back(it->second);
		}
	}

	return polygons;
}

sf::Vector2f Quadtree::checkCollisions(const math::Polygon& polygon, const sf::Vector2f& velocity, std::vector<unsigned int>& indices) const
{
	sf::Vector2f vel = velocity;
	for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
	{
		if (std::find(indices.begin(), indices.end(), it->first) == indices.end())
		{
			indices.push_back(it->first);
			auto intersection = math::SAT(polygon, *it->second, vel);

			if (intersection.willIntersect)
				vel = sf::Vector2f(vel.x + intersection.minimumTranslationVector.x, vel.y + intersection.minimumTranslationVector.y);
		}
	}

	return vel;
}

sf::Vector2f Quadtree::checkCollisions(const math::Polygon& polygon, JumpCheck& jumpCheckLeft, JumpCheck& jumpCheckBottom, JumpCheck& jumpCheckRight, const sf::Vector2f& velocity, std::vector<unsigned int>& indices) const
{
	sf::Vector2f vel = velocity;
	for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
	{
		if (std::find(indices.begin(), indices.end(), it->first) == indices.end())
		{
			indices.push_back(it->first);
			auto intersection = math::SAT(polygon, *it->second, vel);

			if (intersection.willIntersect)
				vel = sf::Vector2f(vel.x + intersection.minimumTranslationVector.x, vel.y + intersection.minimumTranslationVector.y);

			intersection = math::SAT(jumpCheckLeft.polygon, *it->second, vel);
			if (intersection.willIntersect || intersection.intersect)
				jumpCheckLeft.grounded = true;

			intersection = math::SAT(jumpCheckBottom.polygon, *it->second, vel);
			if (intersection.willIntersect || intersection.intersect)
				jumpCheckBottom.grounded = true;
			
			intersection = math::SAT(jumpCheckRight.polygon, *it->second, vel);
			if (intersection.willIntersect || intersection.intersect)
				jumpCheckRight.grounded = true;
		}
	}

	return vel;
}

void Quadtree::populateWithPolygons(std::vector<std::weak_ptr<math::Polygon>>& polygons, std::vector<unsigned int>& indices) const
{
	if (m_hasChildren)
	{
		for (int i = 0; i < 4; ++i)
			m_children[i]->populateWithPolygons(polygons, indices);
	}
	else
	{
		indices.reserve(m_polygons.size());
		polygons.reserve(m_polygons.size());

		for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
		{
			if (std::find(indices.begin(), indices.end(), it->first) == indices.end())
			{
				indices.push_back(it->first);
				polygons.push_back(it->second);
			}
		}
	}
}


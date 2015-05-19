#include "navigation/NavGraph.hpp"


NavGraph::NavGraph()
{
}

NavGraph::~NavGraph()
{
}

void NavGraph::expand(const uint8_t& size, const sf::Vector2f& position)
{
	if (size <= 0)
		return;

	m_nodes.push_back(position);

	if (auto quadtree = m_quadtree.lock())
	{
		auto objects = quadtree->getObjects(sf::Rect<int>(position.x - STEP, position.y - STEP, STEP*2, STEP*2));
	}
}

void NavGraph::setQuadtree(const std::weak_ptr<Quadtree> quadtree)
{
	m_quadtree = quadtree;
}

bool NavGraph::viableNode(const sf::Vector2f& parent, const sf::Vector2f& child, const std::vector<std::weak_ptr<Object>>& objects) const
{
	if (std::find(m_nodes.begin(), m_nodes.end(), child) != m_nodes.end())
		return true;

	for (int i = 0; i < objects.size(); ++i)
	{
		if (auto object = objects[i].lock())
			if (math::lineIntersectsPolygon(parent, child, object->getPolygon()))
				return false;
	}

	return true;
}


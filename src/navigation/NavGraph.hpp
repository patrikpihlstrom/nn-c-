#pragma once

#include <vector>
#include <memory>

#include <SFML/System/Vector2.hpp>

#include "world/Quadtree.hpp"


const static uint8_t STEP = 16;

class NavGraph
{
public:
	NavGraph();
	~NavGraph();

	void expand(const uint8_t& size, const sf::Vector2f& position);

	void setQuadtree(const std::weak_ptr<Quadtree> quadtree);

private:
	std::vector<sf::Vector2f> m_nodes;

	std::weak_ptr<Quadtree> m_quadtree;

	bool viableNode(const sf::Vector2f& parent, const sf::Vector2f& child, const std::vector<std::weak_ptr<Object>>& objects) const;
};


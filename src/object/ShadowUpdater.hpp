#pragma once

#include <SFML/Graphics.hpp>

#include "map/Quadtree.hpp"


class ShadowUpdater
{
public:
	ShadowUpdater();
	~ShadowUpdater();

	void setQuadtree(std::weak_ptr<Quadtree> quadtree);
	void updateShadows(const sf::Rect<int>& boundingBox);

private:
	std::weak_ptr<Quadtree> m_quadtree;
	std::map<unsigned short, sf::Vector2f> m_lightPositions;
};


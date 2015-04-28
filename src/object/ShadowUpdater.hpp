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
	std::shared_ptr<Light> insert(const Light& light);

private:
	std::weak_ptr<Quadtree> m_quadtree;
	std::map<unsigned short, sf::Vector2f> m_lightPositions;
	std::vector<std::shared_ptr<Light>> m_lights;
};


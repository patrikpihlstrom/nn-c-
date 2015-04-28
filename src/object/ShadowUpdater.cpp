#include "ShadowUpdater.hpp"
#include <iostream>


ShadowUpdater::ShadowUpdater()
{
}

ShadowUpdater::~ShadowUpdater()
{
}

void ShadowUpdater::setQuadtree(std::weak_ptr<Quadtree> quadtree)
{
	m_quadtree = quadtree;
}

void ShadowUpdater::updateShadows(const sf::Rect<int>& boundingBox)
{
	if (auto quadtree = m_quadtree.lock())
	{
		for (int i = 0; i < m_lights.size(); ++i)
		{
			m_lights[i]->update();

			if (m_lightPositions[m_lights[i]->getId().id] != m_lights[i]->getPosition())
			{
				m_lightPositions[m_lights[i]->getId().id] = m_lights[i]->getPosition();
				m_lights[i]->clear();
				std::vector<GameObject*> gameObjects;
				quadtree->getGameObjects(gameObjects, m_lights[i]->getBoundingBox());
				std::cout << gameObjects.size() << std::endl;
				for (int j = 0; j < gameObjects.size(); ++j)
					m_lights[i]->accountForObject(gameObjects[j]->getPolygon());

				m_lights[i]->buildLightShape();
			}
		}
	}
}

std::shared_ptr<Light> ShadowUpdater::insert(const Light& light)
{
	m_lights.push_back(std::shared_ptr<Light>(new Light(light)));
	return m_lights.back();
}


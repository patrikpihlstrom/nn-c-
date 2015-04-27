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
		std::vector<Light*> lights;
		quadtree->getLights(lights, boundingBox);

		for (int i = 0; i < lights.size(); ++i)
		{
			lights[i]->update();

			if (m_lightPositions[lights[i]->getId().id] != lights[i]->getPosition())
			{
				m_lightPositions[lights[i]->getId().id] = lights[i]->getPosition();
				lights[i]->clear();
				std::vector<GameObject*> gameObjects;
				quadtree->getGameObjects(gameObjects, lights[i]->getBoundingBox());
				for (int j = 0; j < gameObjects.size(); ++j)
					lights[i]->accountForObject(gameObjects[j]->getPolygon());

				lights[i]->buildLightShape();
			}
		}
	}
}


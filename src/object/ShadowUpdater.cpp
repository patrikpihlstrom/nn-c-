#include "ShadowUpdater.hpp"


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

void ShadowUpdater::updateShadows(const math::Polygon& viewport)
{
	if (auto quadtree = m_quadtree.lock())
	{
		std::vector<Light*> lights;
		quadtree->getLights(lights, viewport);

		std::vector<GameObject*> gameObjects;
		quadtree->getGameObjects(gameObjects, viewport);

		for (int i = 0; i < lights.size(); ++i)
		{
			if (m_lightPositions[lights[i]->getId().id] != lights[i]->getPosition())
			{
				m_lightPositions[lights[i]->getId().id] = lights[i]->getPosition();
				lights[i]->clear();

				for (int j = 0; j < gameObjects.size(); ++j)
				{
					lights[i]->accountForObject(gameObjects[j]->getPolygon());
				}

				lights[i]->buildLightShape();
			}
		}
	}
}


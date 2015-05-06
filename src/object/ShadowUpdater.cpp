#include "ShadowUpdater.hpp"
#include <iostream>


ShadowUpdater::ShadowUpdater()
{
}

ShadowUpdater::~ShadowUpdater()
{
}

void ShadowUpdater::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_lights.size(); ++i)
		m_lights[i]->draw(target, states);
}

void ShadowUpdater::setQuadtree(std::weak_ptr<Quadtree> quadtree)
{
	m_quadtree = quadtree;
}

void ShadowUpdater::setActorManager(std::weak_ptr<ActorManager> actorManager)
{
	m_actorManager = actorManager;
}

void ShadowUpdater::updateShadows(const sf::Rect<int>& boundingBox)
{
	if (auto quadtree = m_quadtree.lock())
	{
		if (auto actorManager = m_actorManager.lock())
		{
			for (int i = 0; i < m_lights.size(); ++i)
			{
				m_lights[i]->update();

				if (m_lightPositions[m_lights[i]->getId().id] != m_lights[i]->getPosition())
				{
					m_lightPositions[m_lights[i]->getId().id] = m_lights[i]->getPosition();
					m_lights[i]->clear();

					auto actors = actorManager->getUniqueActors(boundingBox);
					
					for (int j = 0; j < actors.size(); ++j)
					{
						if (auto actor = actors[j].lock())
							m_lights[i]->accountForObject(actor->getPolygon());
					}

					std::vector<GameObject*> gameObjects;
					quadtree->getGameObjects(gameObjects, m_lights[i]->getBoundingBox());
					for (int j = 0; j < gameObjects.size(); ++j)
						m_lights[i]->accountForObject(gameObjects[j]->getPolygon());

					m_lights[i]->buildLightShape();
				}
			}
		}
	}
}

std::shared_ptr<Light> ShadowUpdater::insert(const Light& light)
{
	m_lights.push_back(std::shared_ptr<Light>(new Light(light)));
	return m_lights.back();
}


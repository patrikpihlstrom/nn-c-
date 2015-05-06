#pragma once

#include <SFML/Graphics.hpp>

#include "map/Quadtree.hpp"
#include "actor/ActorManager.hpp"


class ShadowUpdater : public sf::Drawable
{
public:
	ShadowUpdater();
	~ShadowUpdater();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setQuadtree(std::weak_ptr<Quadtree> quadtree);
	void setActorManager(std::weak_ptr<ActorManager> actorManager);
	void updateShadows(const sf::Rect<int>& boundingBox);
	std::shared_ptr<Light> insert(const Light& light);

private:
	std::weak_ptr<Quadtree> m_quadtree;
	std::weak_ptr<ActorManager> m_actorManager;
	std::map<unsigned short, sf::Vector2f> m_lightPositions;
	std::vector<std::shared_ptr<Light>> m_lights;
};


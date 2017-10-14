#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"
#include "actor/NPCSpawner.hpp"
#include "actor/NPCSpawnEvent.hpp"
#include "application/TextureHolder.hpp"
#include "application/Camera.hpp"
#include "object/ObjectIdTracker.hpp"
#include "object/Object.hpp"
#include "world/Quadtree.hpp"


class World : public sf::Drawable
{
public:
	World();
	World(const long& seed);
	~World();

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	long m_seed;

	struct Hash
	{
		std::size_t operator()(const sf::Vector2i& key) const
		{
			int hash = 51;
			hash = ((hash + key.x) << 5) - (hash + key.x);
			hash = ((hash + key.y) << 5) - (hash + key.y);
			return hash;
		}
	};

	std::shared_ptr<Quadtree> m_quadtree;
	std::vector<Object> m_objects;
};


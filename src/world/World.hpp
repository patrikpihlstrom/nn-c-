#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"
#include "actor/NPCSpawner.hpp"
#include "application/TextureHolder.hpp"
#include "application/Camera.hpp"
#include "object/ObjectIdTracker.hpp"
#include "object/Object.hpp"
#include "world/Region.hpp"


class World : public sf::Drawable
{
public:
	World();
	World(const long& seed);
	~World();

	void update(const float& deltaTime, const sf::Vector2i& playerPosition);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	long m_seed;

	void removeRegions(const sf::Vector2i& position);

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

	std::unordered_map<sf::Vector2i, Region, Hash> m_regions;
};


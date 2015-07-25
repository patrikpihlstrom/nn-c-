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
};


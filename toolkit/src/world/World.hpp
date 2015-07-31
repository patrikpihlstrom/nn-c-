#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "application/TextureHolder.hpp"


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


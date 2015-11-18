#pragma once

#include <SFML/System/Vector2.hpp>

#include "application/Math.hpp"
#include "world/Chunk.hpp"


const float DISTANCE = /*2.8284271247461903;//11.313708498984761;*/5.656854249492381;
const int REGION_SIZE = 256;

class Region : public sf::Drawable
{
public:
	Region(const sf::Vector2i& position, const int seed);
	~Region();

	sf::Vector2i getPosition() const;
	bool isSpawner() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void initializeChunks();

	sf::Vector2i m_position; // Region-based index (1 = 8192)

//	Chunk m_chunks[8][8];

	unsigned short m_civilization;
	sf::Color m_color;

	bool m_spawner;
};


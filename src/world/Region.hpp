#pragma once

#include <SFML/System/Vector2.hpp>

#include "application/Math.hpp"
#include "world/Chunk.hpp"


enum class RegionType
{
	Barren = 0,
	Town = 1,
	Crossroads = 2,
};

const float DISTANCE = 5.656854249492381;
const int REGION_SIZE = 4096;

class Region : public sf::Drawable
{
public:
	Region(const sf::Vector2i& position, const int seed);
	~Region();

	RegionType getType() const;
	sf::Vector2i getPosition() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void initializeChunks();

	RegionType m_type;
	sf::Vector2i m_position; // Region-based index (1 = 4096)
	Chunk m_chunks[8][8];
};


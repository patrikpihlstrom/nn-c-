#include "world/Region.hpp"


Region::Region(const sf::Vector2i& position, const int seed) :
	m_position(position)
{
	srand(position.x+(position.y*seed));
	m_type = (RegionType)(rand()%3);
	initializeChunks();
}

Region::~Region()
{
}

void Region::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0, j = 0; i < 8; ++i)
		for (j = 0; j < 8; ++j)
			m_chunks[i][j].draw(target, states);
}

void Region::initializeChunks()
{
	for (int i = 0, j = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			m_chunks[i][j] = Chunk({m_position.x*REGION_SIZE + i*CHUNK_SIZE, m_position.y*REGION_SIZE + j*CHUNK_SIZE});
			m_chunks[i][j].setWeight((int)((((math::distance<float>({3.5f, 3.5f}, {(float)i, (float)j}) - DISTANCE))/DISTANCE)*(static_cast<float>(rand()%100)))*-1);
		}
	}
}

sf::Vector2i Region::getPosition() const
{
	return m_position;
}


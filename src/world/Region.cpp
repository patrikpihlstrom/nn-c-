#include "world/Region.hpp"


Region::Region(const sf::Vector2i& position, const int seed) :
	m_position(position)
{
	auto _seed = (position.x+(position.y*seed));
	srand(_seed);

	if ((int)std::sqrt(_seed*rand()%2 + rand()%10 + 1)%10 == 0)
		m_spawner = true;
	else
		m_spawner = false;

	m_color.r = rand()%50 + 205;
	m_color.g = 149;//255*(static_cast<float>(rand()%100)/100);
	m_color.b = 108;//255*(static_cast<float>(rand()%100)/100);
	//auto region = (rand()%100);

	//m_type = region <= 60 ? RegionType::Barren:(region <= 90 ? RegionType::Crossroads:(town ? RegionType::Town:RegionType::Barren));
	
	initializeChunks();
}

Region::~Region()
{
}

void Region::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape shape;
	shape.setPosition(m_position.x*REGION_SIZE, m_position.y*REGION_SIZE);
	shape.setSize({REGION_SIZE, REGION_SIZE});
	shape.setFillColor(m_color);
	target.draw(shape, states);
	/*for (int i = 0, j = 0; i < 8; ++i)
		for (j = 0; j < 8; ++j)
			m_chunks[i][j].draw(target, states);
			*/
}

void Region::initializeChunks()
{
	/*sf::Color barren = {255, 0, 0, 100};
	sf::Color town = {0, 255, 0, 100};
	sf::Color crossroads = {0, 0, 255, 100};
	for (int i = 0, j = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			m_chunks[i][j] = Chunk({m_position.x*REGION_SIZE + i*CHUNK_SIZE, m_position.y*REGION_SIZE + j*CHUNK_SIZE});
			m_chunks[i][j].setWeight((int)((((math::distance<float>({3.5f, 3.5f}, {(float)i, (float)j}) - DISTANCE))/DISTANCE)*(static_cast<float>(rand()%100)))*-1);
			m_chunks[i][j].setColor(m_color);
			//m_chunks[i][j].setColor(getType() == RegionType::Barren ? barren:(getType() == RegionType::Crossroads ? crossroads:town));
		}
	}*/
}

sf::Vector2i Region::getPosition() const
{
	return m_position;
}

bool Region::isSpawner() const
{
	return m_spawner;
}


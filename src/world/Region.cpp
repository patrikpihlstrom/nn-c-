#include "world/Region.hpp"


Region::Region(const sf::Vector2i& position, const int seed) :
	m_position(position)
{
	auto _seed = (position.x+(position.y*seed));
	std::cout << _seed << std::endl;
	srand(_seed);
	//auto region = (rand()%100);

	//m_type = region <= 60 ? RegionType::Barren:(region <= 90 ? RegionType::Crossroads:(town ? RegionType::Town:RegionType::Barren));
	
	m_civilization = rand()%100;

	if (m_civilization >= 65 && (position.x%(rand()%5 + 1) == 0 && position.y%(rand()%5 + 1) == 0))
		m_civilization = rand()%50 + 50;
	else
		m_civilization = rand()%20;

	m_color = {(unsigned char)(((float)m_civilization/100)*255), 0, 0, 100};

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
	/*sf::Color barren = {255, 0, 0, 100};
	sf::Color town = {0, 255, 0, 100};
	sf::Color crossroads = {0, 0, 255, 100};
	*/
	for (int i = 0, j = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			m_chunks[i][j] = Chunk({m_position.x*REGION_SIZE + i*CHUNK_SIZE, m_position.y*REGION_SIZE + j*CHUNK_SIZE});
			m_chunks[i][j].setWeight((int)((((math::distance<float>({3.5f, 3.5f}, {(float)i, (float)j}) - DISTANCE))/DISTANCE)*(static_cast<float>(rand()%50)))*-1);
			m_chunks[i][j].setColor(m_color);
			//m_chunks[i][j].setColor(getType() == RegionType::Barren ? barren:(getType() == RegionType::Crossroads ? crossroads:town));
		}
	}
}

sf::Vector2i Region::getPosition() const
{
	return m_position;
}


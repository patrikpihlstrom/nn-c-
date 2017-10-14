#include "World.hpp"


World::World()
{
}

World::World(const long& seed) :
	m_seed(seed)
{
	std::cout << "SEED: " << m_seed << std::endl;
	srand(m_seed);
	for (int i = 0; i < 10; ++i)
	{
		Object object = Object();
	}
}

World::~World()
{
}

void World::update(const float& deltaTime)
{
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		target.draw(*it, states);
	}
}


#include "World.hpp"


World::World()
{
}

World::World(const long& seed) :
	m_seed(seed)
{
	std::cout << "SEED: " << m_seed << std::endl;
	srand(m_seed);
}

World::~World()
{
}

void World::update(const float& deltaTime, const sf::Vector2i& playerPosition)
{
	removeRegions(playerPosition);

	for (int i = 0, j = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			sf::Vector2i pos = {playerPosition.x/REGION_SIZE + i - 1, playerPosition.y/REGION_SIZE + j - 1};
			if (m_regions.find(pos) == m_regions.end())
			{
				Region region = Region(pos, m_seed);
				m_regions.insert(std::pair<sf::Vector2i, Region>(pos, region));
			}
		}
	}
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_regions.begin(); it != m_regions.end(); ++it)
	{
		target.draw(it->second, states);
	}
}

void World::removeRegions(const sf::Vector2i& position)
{
	auto it = m_regions.begin();
	while (it != m_regions.end())
	{
		if (it->second.getPosition().x + REGION_SIZE < position.x - 860 || it->second.getPosition().x > position.x + 860 || it->second.getPosition().y + REGION_SIZE < position.y - 860 || it->second.getPosition().y > position.y + 860)
			it = m_regions.erase(it);
		else
			++it;
	}
}

/*std::weak_ptr<ActorManager> World::getActorManager() const
{
	return m_actorManager;
}

std::weak_ptr<Camera> World::getCamera() const
{
	assert(m_playerActor);
	return m_playerActor->getCamera();
}

void World::addObject(const Object& object)
{
	std::shared_ptr<Object >newObject(new Object(object));
	newObject->assign(m_objectIdTracker->addObject());
}
*/


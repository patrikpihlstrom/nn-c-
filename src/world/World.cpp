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

	for (int i = -8, j = -8; i <= 8; ++i)
	{
		for (j = -8; j <= 8; ++j)
		{
			sf::Vector2i pos = {playerPosition.x/REGION_SIZE + i, playerPosition.y/REGION_SIZE + j};
			if (m_regions.find(pos) == m_regions.end())
			{
				Region region = Region(pos, m_seed);
				
				if (region.isSpawner())
					m_spawnEvents.insert(m_spawnEvents.end(), rand()%5 + 3, {"test", {region.getPosition().x*REGION_SIZE, region.getPosition().y*REGION_SIZE}});

				m_regions.insert(std::pair<sf::Vector2i, Region>(pos, region));
			}
		}
	}
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_regions.begin(); it != m_regions.end(); ++it)
		target.draw(it->second, states);
}

void World::removeRegions(const sf::Vector2i& position)
{
	auto it = m_regions.begin();
	while (it != m_regions.end())
	{
		if (it->second.getPosition().x + REGION_SIZE < position.x - 860*8 || it->second.getPosition().x > position.x + 860*8 || it->second.getPosition().y + REGION_SIZE < position.y - 860*8 || it->second.getPosition().y > position.y + 860*8)
			it = m_regions.erase(it);
		else
			++it;
	}
}

std::vector<NPCSpawnEvent> World::getSpawnEvents() const
{
	return m_spawnEvents;
}

void World::clearSpawnEvents()
{
	m_spawnEvents.clear();
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


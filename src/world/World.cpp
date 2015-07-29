#include "World.hpp"


World::World()
{
}

World::World(const long& seed) :
	m_seed(seed)
{
	std::cout << "SEED: " << m_seed << std::endl;
}

World::~World()
{
}

void World::update(const float& deltaTime)
{
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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


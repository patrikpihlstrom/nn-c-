#include "World.hpp"


World::World()
{
}

World::World(const long& seed) :
	m_seed(seed)
{
	m_quadtree = std::shared_ptr<Quadtree>(new Quadtree({0, 0, 1600, 900}, 0));
	std::cout << "SEED: " << m_seed << std::endl;
	srand(m_seed);
	auto object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(0, 0);
	object->setSize({20, 900});
	m_objects.push_back(object);
	object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(1580, 0);
	object->setSize({20, 900});
	m_objects.push_back(object);
	object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(0, 300);
	object->setSize({1600, 20});
	m_objects.push_back(object);
	object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(0, 580);
	object->setSize({1600, 20});
	m_objects.push_back(object);

	/*object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(800, 450);
	object->setSize({32, 32});
	m_objects.push_back(object);*/

	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		m_quadtree->insert(*it);
	}

	auto actor = std::shared_ptr<NNActor>(new NNActor());
	actor->setPosition(800, 450);
	m_actorManager.addActor(actor);
}

World::~World()
{
}

void World::update(const float& deltaTime)
{
	//m_quadtree->update();
	m_actorManager.update(deltaTime, m_quadtree);
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		target.draw(*(*it), states);
	}

	target.draw(m_actorManager, states);

	//m_quadtree->draw(target, states);
}


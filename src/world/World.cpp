#include "World.hpp"


World::World()
{
}

World::World(const long& seed) :
	m_seed(seed)
{
	float width = 1280, height = 720;
	m_quadtree = std::shared_ptr<Quadtree>(new Quadtree({0, 0, (int)width, (int)height}, 0));
	std::cout << "SEED: " << m_seed << std::endl;
	srand(m_seed);
	auto object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(0, 0);
	object->setSize({20, height});
	m_objects.push_back(object);
	object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(width - 20, 0);
	object->setSize({20, height});
	m_objects.push_back(object);
	object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(0, 0);
	object->setSize({width, 20});
	m_objects.push_back(object);
	object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(0, height - 20);
	object->setSize({width, 20});
	m_objects.push_back(object);

	for (int i = 0; i < 0; ++i)
	{
		object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
		object->setPosition(rand()%((int)width-200 + 1) + 100.f, rand()%((int)height-200 + 1) + 100.f);
		object->setSize({rand()%(10-75 + 1) + 10.f, rand()%(10-75 + 1) + 10.f});
		m_objects.push_back(object);
	}

	/*object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
	object->setPosition(800, 450);
	object->setSize({32, 32});
	m_objects.push_back(object);*/

	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		m_quadtree->insert(*it);
	}

	for (int i = 0; i < 1; ++i)
	{
		auto actor = std::shared_ptr<NNActor>(new NNActor());
		actor->setPosition(width/2, height/2);
		m_actorManager.addActor(actor);
	}
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


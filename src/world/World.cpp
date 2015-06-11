#include "World.hpp"


World::World()
{
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_rockGenerator.reset(new RockGenerator());
	m_pathGenerator.reset(new PathGenerator());
	m_actorManager.reset(new ActorManager());
	m_npcSpawner.reset(new NPCSpawner());
	m_camera.reset(new Camera());
	m_camera->setSize(1600, 900);
	m_textureHolder->loadTextures("assets/Textures.lst");
	m_sprite.setTexture(*m_textureHolder->getTexture("test").lock());

	std::shared_ptr<PlayerActor> playerActor;
	playerActor.reset(new PlayerActor());

	playerActor->setPosition(0, 0);
	playerActor->setSize(16, 72);
	playerActor->setTexture(m_textureHolder->getTexture("char"));
	playerActor->assign(m_actorIdTracker->addActor());
	m_camera->trackActor(playerActor);

	m_actorManager->addActor(playerActor);

	for (int i = 0; i < 0; ++i)
		m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), {(float)(rand()%100), (float)(rand()%100)}, *m_actorManager, *m_actorIdTracker);

	Object object;
	int rocks = 0;
	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(3 + rand()%5, {2, 1}, {std::cos(angle)*(rand()%2000), std::sin(angle)*(rand()%2000)});

		object.setPolygon(polygon);
		addObject(object);
	}

	float deviation = M_PI/4;
	m_pathGenerator->setTexture(m_textureHolder->getTexture("thing"));
	m_paths.push_back(m_pathGenerator->generatePath({0, 0}, 4, {0, -1}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {.125, -.875}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {.25, -.75}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {.5, -.625}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {.625, -.5}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {.75, -.25}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {.875, -.125}, 32, deviation));
	m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 4, {1, 0}, 32, deviation));

//	for (int i = 0; i < 1; ++i)
//		m_paths.push_back(m_pathGenerator->generatePath(m_paths.back().getLast(), 8, {-1, 0}, 32, M_PI/16));
}

World::~World()
{
}

void World::update(const float& deltaTime, const sf::RenderWindow& window)
{
	float dt = deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_npcSpawner->spawn("test", m_textureHolder->getTexture("char"), m_actorManager->getActor({0}).lock()->getPosition(), *m_actorManager, *m_actorIdTracker);

	if (!m_zooming)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			m_camera->zoom(1.1f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			m_camera->zoom(.9f);
	}
	m_zooming = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));

	m_camera->update();
	m_actorManager->update(dt, sf::Rect<int>(m_camera->getCenter().x - m_camera->getSize().x, m_camera->getCenter().y - m_camera->getSize().y, m_camera->getSize().x*2, m_camera->getSize().y*2));
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(m_sprite, states);

	for (int i = 0; i < m_paths.size(); ++i)
		target.draw(m_paths[i], states);

	m_quadtree->draw(m_camera->getBounds<int>(), target, states);
	m_actorManager->draw(target, states);
}

std::weak_ptr<Quadtree> World::getQuadtree() const
{
	return m_quadtree;
}

std::weak_ptr<ActorManager> World::getActorManager() const
{
	return m_actorManager;
}

Camera World::getCamera() const
{
	return *m_camera;
}

void World::addObject(const Object& object)
{
	std::shared_ptr<Object >newObject(new Object(object));
	newObject->assign(m_objectIdTracker->addObject());
	m_quadtree->insert(newObject);
}


#include "World.hpp"


World::World()
{
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_rockGenerator.reset(new RockGenerator());
	m_actorManager.reset(new ActorManager());
	m_npcSpawner.reset(new NPCSpawner());
	m_camera.reset(new Camera());
	m_camera->setSize(1600, 900);
	m_textureHolder->loadTextures("assets/Textures.lst");

	std::shared_ptr<PlayerActor> playerActor;
	playerActor.reset(new PlayerActor());

	playerActor->setPosition(0, 0);
	playerActor->setSize(32);
	playerActor->assign(m_actorIdTracker->addActor());
	m_camera->trackActor(playerActor);

	m_actorManager->addActor(playerActor);

	for (int i = 0; i < 25; ++i)
		m_npcSpawner->spawn("test", {0, 0}, *m_actorManager, *m_actorIdTracker);

	Object object;
	int rocks = 50;
	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(3 + rand()%5, {2, 1}, {std::cos(angle)*(rand()%250), std::sin(angle)*(rand()%250)});

		object.setPolygon(polygon);
		addObject(object);
	}
}

World::~World()
{
}

void World::update(const float& deltaTime, const sf::RenderWindow& window)
{
	float dt = deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (!space)
		{
			m_npcSpawner->spawn("test", {32, 32}, *m_actorManager, *m_actorIdTracker);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				m_camera->zoom(1.1f);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				m_camera->zoom(.9f);
		}

		dt /= 2;
	}
	space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	m_camera->update();
	m_actorManager->update(dt, m_camera->getBounds<int>());
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_quadtree->draw(m_camera->getBounds<int>(), target, states);
	m_actorManager->draw(target, states);

	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(32, 32));
	rect.setPosition(m_actorManager->getActor({0}).lock()->getPosition().x, m_actorManager->getActor({0}).lock()->getPosition().y);
	rect.setOutlineThickness(2);
	rect.setOutlineColor(sf::Color(255, 0, 255));
	rect.setFillColor(sf::Color(255, 0, 255, 100));

	target.draw(rect, states);
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


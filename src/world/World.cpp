#include "World.hpp"


World::World()
{
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_rockGenerator.reset(new RockGenerator());
	m_actorManager.reset(new ActorManager());
	m_camera.reset(new Camera());
	m_camera->setSize(1600, 900);
	m_textureHolder->loadTextures("assets/Textures.lst");

	std::shared_ptr<PlayerActor> playerActor;
	playerActor.reset(new PlayerActor());

	playerActor->setBounds({0, 0, 32, 32});
	playerActor->setPosition(0, 0);

	math::Polygon polygon;
	polygon.addPoint(sf::Vector2f(0, 0));
	polygon.addPoint(sf::Vector2f(32, 0));
	polygon.addPoint(sf::Vector2f(32, 32));
	polygon.addPoint(sf::Vector2f(0, 32));
	polygon.constructEdges();

	sf::ConvexShape shape;
	shape.setPointCount(4);
	shape.setPoint(0, sf::Vector2f(0, 0));
	shape.setPoint(1, sf::Vector2f(32, 0));
	shape.setPoint(2, sf::Vector2f(32, 32));
	shape.setPoint(3, sf::Vector2f(0, 32));
	shape.setFillColor(sf::Color(255, 0, 0));
	playerActor->setShape(shape);
	playerActor->setPolygon(polygon);
	playerActor->assign(m_actorIdTracker->addActor());
	m_camera->trackActor(playerActor);

	m_actorManager->addActor(playerActor);

	for (int i = 0; i < 25; ++i)
	{
		uint8_t size = 4 + rand()%64;

		std::shared_ptr<NPCActor> npcActor;
		npcActor.reset(new NPCActor("test", sf::Vector2f(0, 0)));

		npcActor->setBounds({0, 0, size, size});
		npcActor->setPosition(0, 0);

		polygon.clear();
		polygon.addPoint(sf::Vector2f(0, 0));
		polygon.addPoint(sf::Vector2f(size, 0));
		polygon.addPoint(sf::Vector2f(size, size));
		polygon.addPoint(sf::Vector2f(0, size));
		polygon.constructEdges();

		shape.setPoint(0, sf::Vector2f(0, 0));
		shape.setPoint(1, sf::Vector2f(size, 0));
		shape.setPoint(2, sf::Vector2f(size, size));
		shape.setPoint(3, sf::Vector2f(0, size));
		shape.setFillColor(sf::Color(255, 0, 0));
		npcActor->setShape(shape);
		npcActor->setPolygon(polygon);
		npcActor->assign(m_actorIdTracker->addActor());
		npcActor->registerLuaStateMachine();

		m_actorManager->addActor(npcActor);
	}

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
	/*if (auto light = m_light.lock())
	{
		light->move(sf::Mouse::getPosition(window).x + m_camera->getCenter().x - m_camera->getSize().x/2 - light->getPosition().x, sf::Mouse::getPosition(window).y + m_camera->getCenter().y - m_camera->getSize().y/2 - light->getPosition().y);
	}
	*/

	float dt = deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !space)
	{
		/*Light light;
		light.assign(m_objectIdTracker->addObject());
		//light.setPosition(sf::Vector2f(1000 + std::rand()%2000, 500 + std::rand()%1000));
		light.setTexture(m_textureHolder->getTexture("light"));
		light.setColor(sf::Color(std::rand()%255, std::rand()%255, std::rand()%255));
		//light.setColor(sf::Color(255, 150, 100, 100));
		m_light = m_shadowUpdater->insert(light);
		*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			m_camera->zoom(1.1f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			m_camera->zoom(.9f);
		else
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


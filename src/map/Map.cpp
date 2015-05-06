#include "Map.hpp"


Map::Map()
{
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_actorIdTracker.reset(new ActorIdTracker());
	m_shadowUpdater.reset(new ShadowUpdater());
	m_rockGenerator.reset(new RockGenerator());
	m_actorManager.reset(new ActorManager());
	m_camera.reset(new Camera());
	m_camera->setSize(1600, 900);

	PlayerActor playerActor;
	playerActor.setBounds({0, 0, 32, 32});
	playerActor.setPosition(0, 0);
	sf::ConvexShape shape;
	shape.setPointCount(4);
	shape.setPoint(0, sf::Vector2f(0, 0));
	shape.setPoint(1, sf::Vector2f(32, 0));
	shape.setPoint(2, sf::Vector2f(32, 32));
	shape.setPoint(3, sf::Vector2f(0, 32));
	shape.setFillColor(sf::Color(255, 0, 0));
	playerActor.setShape(shape);
	playerActor.assign(m_actorIdTracker->addActor());

	m_actorManager->createNewPlayerActor(playerActor);
	m_camera->trackActor(m_actorManager->getActor(playerActor.getId()));

	m_shadowUpdater->setQuadtree(getQuadtree());
	m_textureHolder->loadTextures("assets/Textures.lst");

	Decal decal;
	math::Polygon polygon;
	polygon.addPoint(sf::Vector2f(0, 0));
	polygon.addPoint(sf::Vector2f(1055*2, 0));
	polygon.addPoint(sf::Vector2f(1055*2, 683));
	polygon.addPoint(sf::Vector2f(0, 683));

	decal.setPolygon(polygon);
	decal.setColor(sf::Color(255, 255, 255));
	decal.rotate(-M_PI/7);
	decal.setColor(sf::Color(55, 0, 55));
	addDecal(decal);

	GameObject gameObject;
	int rocks = 750;
	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(4, {2, 1}, {std::cos(angle)*(rand()%2500), std::sin(angle)*(rand()%2500)});

		gameObject.setPolygon(polygon);
		addObject(gameObject);
	}
}

Map::~Map()
{
	m_quadtree.reset();
}

void Map::update(const float& deltaTime, const sf::RenderWindow& window)
{
	if (auto light = m_light.lock())
	{
		light->move(sf::Mouse::getPosition(window).x + m_camera->getCenter().x - m_camera->getSize().x/2 - light->getPosition().x, sf::Mouse::getPosition(window).y + m_camera->getCenter().y - m_camera->getSize().y/2 - light->getPosition().y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !space)
	{
		Light light;
		light.assign(m_objectIdTracker->addObject());
		//light.setPosition(sf::Vector2f(1000 + std::rand()%2000, 500 + std::rand()%1000));
		light.setTexture(m_textureHolder->getTexture("light"));
		light.setColor(sf::Color(std::rand()%255, std::rand()%255, std::rand()%255));
		//light.setColor(sf::Color(255, 150, 100, 100));
		m_light = m_shadowUpdater->insert(light);
	}
	space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	m_camera->update();

	m_actorManager->update(deltaTime);

	m_shadowUpdater->updateShadows(sf::Rect<int>(m_camera->getCenter().x - m_camera->getSize().x/2, m_camera->getCenter().y - m_camera->getSize().y/2, m_camera->getSize().x, m_camera->getSize().y));
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_decals.size(); ++i)
	{
		if (auto decal = m_decals[i].lock())
			decal->draw(target, states);
	}

	m_shadowUpdater->draw(target, states);
	
	for (int i = 0; i < m_gameObjects.size(); ++i)
	{
		if (auto gameObject = m_gameObjects[i].lock())
			gameObject->draw(target, states);
	}

	m_actorManager->draw((sf::Rect<int>)m_camera->getViewport(), target, states);
}

std::weak_ptr<Quadtree> Map::getQuadtree() const
{
	return m_quadtree;
}

void Map::addObject(GameObject& object)
{
	object.assign(m_objectIdTracker->addObject());
	m_quadtree->insert(object);
	m_gameObjects.push_back(m_quadtree->getObject(object.getId()));
}

void Map::addDecal(Decal& decal)
{
	decal.assign(m_objectIdTracker->addObject());
	m_quadtree->insert(std::shared_ptr<Object>(new Decal(decal)));
	m_decals.push_back(m_quadtree->getObject(decal.getId()));
}

Camera Map::getCamera() const
{
	return *m_camera;
}


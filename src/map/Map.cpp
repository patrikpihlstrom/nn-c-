#include "Map.hpp"


Map::Map()
{
	m_quadtree.reset(new Quadtree(sf::Rect<int>(-2048*5, -2048*5, 2048*10, 2048*10), 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_shadowUpdater.reset(new ShadowUpdater());
	m_rockGenerator.reset(new RockGenerator());

	m_shadowUpdater->setQuadtree(getQuadtree());
	m_textureHolder->loadTextures("level.mtl");

	Decal decal;
	decal.setTexture(m_textureHolder->getTexture("Decal"));
	math::Polygon polygon;
	polygon.addPoint(sf::Vector2f(0, 0));
	polygon.addPoint(sf::Vector2f(500, 0));
	polygon.addPoint(sf::Vector2f(500, 200));
	polygon.addPoint(sf::Vector2f(0, 200));
	decal.setPolygon(polygon);
	decal.setColor(sf::Color(230, 230, 230));
	addDecal(decal);


	GameObject gameObject;
	int rocks = 500;
	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(4, {2, 1}, {std::cos(angle)*(rand()%2500), std::sin(angle)*(rand()%2500)});

		gameObject.assign(m_objectIdTracker->addObject());
		gameObject.setPolygon(polygon);

		addObject(gameObject);
	}
}

Map::~Map()
{
	m_quadtree.reset();
}

void Map::update(const sf::RenderWindow& window, const Camera& camera)
{
	if (auto light = m_light.lock())
		light->move(sf::Mouse::getPosition(window).x + camera.getCenter().x - camera.getSize().x/2 - light->getPosition().x, sf::Mouse::getPosition(window).y + camera.getCenter().y - camera.getSize().y/2 - light->getPosition().y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !space)
	{
		Light light;
		light.assign(m_objectIdTracker->addObject());
		//light.setPosition(sf::Vector2f(1000 + std::rand()%2000, 500 + std::rand()%1000));
		light.setTexture(m_textureHolder->getTexture("light"));
		//light.setColor(sf::Color(std::rand()%255, std::rand()%255, std::rand()%255));
		light.setColor(sf::Color(255, 150, 100, 100));
		m_light = m_shadowUpdater->insert(light);
	}
	space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	m_shadowUpdater->updateShadows(sf::Rect<int>(camera.getCenter().x - camera.getSize().x/2, camera.getCenter().y - camera.getSize().y/2, camera.getSize().x, camera.getSize().y));
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
}

std::weak_ptr<Quadtree> Map::getQuadtree() const
{
	return m_quadtree;
}

void Map::addObject(const GameObject& object)
{
	m_quadtree->insert(object);
	m_gameObjects.push_back(m_quadtree->getObject(object.getId()));
}

void Map::addDecal(const Decal& decal)
{
	m_quadtree->insert(std::shared_ptr<Object>(new Decal(decal)));
	m_decals.push_back(m_quadtree->getObject(decal.getId()));
}


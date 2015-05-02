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
	math::Polygon polygon;
	polygon.addPoint(sf::Vector2f(0, 0));
	polygon.addPoint(sf::Vector2f(500, 0));
	polygon.addPoint(sf::Vector2f(500, 100));
	polygon.addPoint(sf::Vector2f(0, 100));
	polygon.constructEdges();
	decal.setPolygon(polygon);
	decal.setColor(sf::Color(255, 0, 255));
	decal.rotate(M_PI);
	decal.setTexture(m_textureHolder->getTexture("Fedora"));
	decal.assign(m_objectIdTracker->addObject());
	m_decals.push_back(std::shared_ptr<Decal>(new Decal(decal)));

	GameObject gameObject;
	
	int rocks = 1000;

	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(4, {3, 2}, {std::cos(angle)*(rand()%2500), std::sin(angle)*(rand()%2500)});

		gameObject.assign(m_objectIdTracker->addObject());
		gameObject.setPolygon(polygon);

		std::shared_ptr<Object> object;
		object.reset(new GameObject(gameObject));

		m_quadtree->insert(object);
		m_gameObjects.push_back(std::shared_ptr<GameObject>(static_cast<GameObject*>(m_quadtree->getObject(object->getId()).get())));
	}
}

Map::~Map()
{
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
		light.setColor(sf::Color(255, 100, 19));
		m_light = m_shadowUpdater->insert(light);
	}
	space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	m_shadowUpdater->updateShadows(sf::Rect<int>(camera.getCenter().x - camera.getSize().x/2, camera.getCenter().y - camera.getSize().y/2, camera.getSize().x, camera.getSize().y));
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_decals.size(); ++i)
	{
		m_decals[i]->draw(target, states);
	}

	m_shadowUpdater->draw(target, states);
	
	for (int i = 0; i < m_gameObjects.size(); ++i)
	{
		m_gameObjects[i]->draw(target, states);
	}
}

std::weak_ptr<Quadtree> Map::getQuadtree() const
{
	return m_quadtree;
}


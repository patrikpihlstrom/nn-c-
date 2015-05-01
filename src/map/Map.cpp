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

	GameObject gameObject;
	
	int rocks = 1000;

	for (int j = 0; j < rocks; ++j)
	{
		float angle = j*((2*M_PI)/rocks);
		auto polygon = m_rockGenerator->getRock(10, {2, 2}, {std::cos(angle)*(rand()%2500), std::sin(angle)*(rand()%2500)});

		gameObject.assign(m_objectIdTracker->addObject());
		gameObject.setPolygon(polygon);

		std::shared_ptr<Object> object;
		object.reset(new GameObject(gameObject));

		m_quadtree->insert(object);
		m_objects.push_back(m_quadtree->getObject(object->getId()));
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
		m_objects.push_back(m_shadowUpdater->insert(light));
		m_light = m_objects.back();
	}
	space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	m_shadowUpdater->updateShadows(sf::Rect<int>(camera.getCenter().x - camera.getSize().x/2, camera.getCenter().y - camera.getSize().y/2, camera.getSize().x, camera.getSize().y));
}

void Map::addObject(GameObject& object, const std::string& mtl)
{
	if (mtl != "(null)" && mtl != "None")
	{
		object.setTexture(m_textureHolder->getTexture(mtl));
		object.setTextureRect(sf::Rect<int>(object.getGlobalBounds()));
	}

	object.assign(m_objectIdTracker->addObject());
	m_quadtree->insert(object);
	m_objects.push_back(m_quadtree->getObject(object.getId()));
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		if (!m_objects[i]->isLight())
			m_objects[i]->draw(target, states);
	}

	m_shadowUpdater->draw(target, states);
}

void Map::load(const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);

	if (file.is_open())
	{
		std::string line;
		std::vector<sf::Vector2f> points;
		float depth = 0;
		while (!file.eof())
		{
			std::getline(file, line);
			if ((line.substr(0, 6) == "usemtl" || line[0] == 'o') && !points.empty()) // new polygon
			{
				math::Polygon polygon;

				if (points.size() > 3)
					points = math::convexHull<float>(points);

				polygon.setPoints(points);
				polygon.constructEdges();
				polygon.setDepth(depth);
				
				GameObject object;
				object.setPolygon(polygon);

				addObject(object, line.substr(0, 6) == "usemtl" ? line.substr(7, line.size() - 1):"");
				depth = 0;
				points.clear();
			}
			else if (line[0] == 'v')
			{
				std::string x, y, z;
				bool _x = true;

				for (int i = 2; i < line.size(); ++i)
				{
					if (line[i] == '.')
					{
						_x = false;
						i += 8;

						while (line[i] != ' ')
						{
							z += line[i];
							i++;
						}

						depth = std::atof(z.c_str());
					}
					else
					{
						if (_x)
							x += line[i];
						else
							y += line[i];
					}
				}

				points.push_back(sf::Vector2f(std::atoi(x.c_str()), std::atoi(y.c_str())));
			}
		}

		file.close();
	}
}

std::weak_ptr<Quadtree> Map::getQuadtree() const
{
	return m_quadtree;
}


#include "Map.hpp"


Map::Map()
{
	m_quadtree.reset(new Quadtree(sf::Vector2f(0, 0), sf::Vector2f(2048*5, 2048*5), false, 0));
	m_textureHolder.reset(new TextureHolder());
	m_objectIdTracker.reset(new ObjectIdTracker());
	m_shadowUpdater.reset(new ShadowUpdater());

	m_shadowUpdater->setQuadtree(getQuadtree());
	m_textureHolder->loadTextures("level.mtl");
}

Map::~Map()
{
}

void Map::update(const sf::RenderWindow& window, const Camera& camera)
{
	math::Polygon viewport;
	viewport.addPoint(sf::Vector2f(-5000, -5000));
	viewport.addPoint(sf::Vector2f(5000, -5000));
	viewport.addPoint(sf::Vector2f(5000, 5000));
	viewport.addPoint(sf::Vector2f(0, 5000));
	viewport.constructEdges();

	if (auto light = m_light.lock())
		light->setPosition((sf::Vector2f)sf::Mouse::getPosition(window) + camera.getCenter() - sf::Vector2f(camera.getSize().x/2, camera.getSize().y/2));

	m_shadowUpdater->updateShadows(viewport);
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
	states.blendMode = sf::BlendMode::BlendMultiply;
	for (int i = 0; i < m_objects.size(); ++i)
		m_objects[i]->draw(target, states);
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

	Light light;

	for (int i = 0; i < 1; ++i)
	{
		light.assign(m_objectIdTracker->addObject());
		light.setPosition(sf::Vector2f(std::rand()%1000 + 800, std::rand()%1000 + 500));
		light.setTexture(m_textureHolder->getTexture("light"));
		light.setColor(sf::Color(std::rand()%255, std::rand()%255, std::rand()%255));
		m_quadtree->insert(light);
		m_objects.push_back(m_quadtree->getObject(light.getId()));
	}
	
	m_light = m_quadtree->getObject(light.getId());

	/*if (m_quadtree->remove(ObjectId{0}))
	{
		auto it = std::find_if(m_objects.begin(), m_objects.end(), Quadtree::compare(ObjectId{0}));
		m_objects.erase(it);
		m_objectIdTracker->removeObject(ObjectId{0});
		m_quadtree->update();
	}
	*/
}

std::weak_ptr<Quadtree> Map::getQuadtree() const
{
	return m_quadtree;
}


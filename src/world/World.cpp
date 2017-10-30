#include "World.hpp"


World::World()
{
}

World::World(const long& seed) :
	m_seed(seed)
{
	m_quadtree = std::shared_ptr<Quadtree>(new Quadtree({-2500, -2500, 5000, 5000}, 0));
	srand(m_seed);

	for (int i = 0; i < 10; ++i)
	{
		auto actor = std::shared_ptr<NNActor>(new NNActor());
		actor->setPosition(0, 0);
		m_actorManager.addActor(actor);
	}

	m_camera.zoom(6.5f);
	m_foodTimer = 0;
	m_sightTimer = 0;
}

World::~World()
{
}

void World::load(const std::string& path)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		std::string line;
		while (!file.eof())
		{
			std::getline(file, line);

			if (!line.empty() && line[0] == 'o') // new object
			{
				auto objectType = getType(line.substr(2));
				math::Polygon polygon;
				std::getline(file, line);
				while (!line.empty() && line[0] != 'u')
				{
					if (!line.empty() && line[0] == 'v' && line.substr(0, 2) != "vn") // is vertex
					{
						auto indices = getAllInstancesOfChar(line, ' ');
						if (indices.size() >= 3)
						{
							auto x = std::atof(line.substr(indices[0] + 1, indices[1] - indices[0] - 1).c_str()), y = std::atof(line.substr(indices[1] + 1, indices[2] - indices[1] - 1).c_str()), z = std::atof(line.substr(indices[2] + 1).c_str());
							polygon.addPoint({(float)x*100, (float)z*100});
						}
					}
					std::getline(file, line);
				}

				if (objectType == ObjectType::obstacle)
				{
					polygon.constructEdges(true);
					auto object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
					object->setType(objectType);
					object->setPosition(0, 0);
					object->setPolygon(polygon);
					m_objects.push_back(object);
					m_quadtree->insert(object);
				}
				else if (objectType == ObjectType::start)
				{
					m_actorManager.setStart(polygon.getPoint(0));
				}
				else if (objectType == ObjectType::finish)
				{
					polygon.constructEdges(true);
					m_actorManager.setFinish(polygon);
				}
			}
		}
	}
}

void World::update(const float& deltaTime)
{
	m_foodTimer -= deltaTime;
	if (m_foodTimer <= 0)
	{
		float angle = ((double)rand()/(RAND_MAX))*M_PI*2;
		float distance = ((double)rand()/(RAND_MAX))*1500.f;
		auto object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
		object->setPosition({distance*std::cos(angle) - distance*std::sin(angle), distance*std::sin(angle) + distance*std::cos(angle)});
		object->setType(ObjectType::food);
		object->dead = false;
		m_objects.push_back(object);
		m_quadtree->insert(object);
		m_foodTimer = 2.f;
	}

	//m_quadtree->update();
	m_actorManager.update(deltaTime, m_quadtree, m_camera);

	auto it = m_objects.begin();
	while (it != m_objects.end())
	{
		if ((*it)->dead)
		{
			it = m_objects.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		target.draw(*(*it), states);
	}

	target.draw(m_actorManager, states);
	m_actorManager.drawNeuralNet(target, states);

	m_quadtree->draw(target, states);
}

sf::View World::getView() const
{
	return m_camera;
}


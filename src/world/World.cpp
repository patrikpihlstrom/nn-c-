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
				auto objectType = line.substr(2);
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
				
				polygon.constructEdges(true);
				auto object = std::shared_ptr<Object>(new Object(m_objectIdTracker.addObject()));
				object->setPosition(0, 0);
				object->setPolygon(polygon);
				m_objects.push_back(object);
				m_quadtree->insert(object);
			}
		}
	}
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
	//m_actorManager.drawNeuralNet(target, states);

	//m_quadtree->draw(target, states);
}




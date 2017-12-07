#include "ActorManager.hpp"


ActorManager::ActorManager()
{
	m_actors.reserve(20);
	for (int i = 0; i < 20; ++i)
	{
		m_actors.push_back(std::shared_ptr<NNActor>(new NNActor()));
		m_actors.back()->setPosition(640, 80);
	}
}

ActorManager::~ActorManager()
{
}

std::weak_ptr<NNActor> ActorManager::getActor(const ActorId& id) const
{
       for (int i = 0; i < m_actors.size(); ++i)
       {
               if (m_actors[i]->getId() == id)
                       return m_actors[i];
       }

       return {};
}

std::weak_ptr<NNActor> ActorManager::getTopActor()
{
       if (m_actors.empty())
       {
               return std::weak_ptr<NNActor>();
       }

       std::sort(m_actors.begin(), m_actors.end(), ActorCompareDistance());
       return *m_actors.begin();
}

void ActorManager::update(const float& deltaTime, std::shared_ptr<Quadtree> quadtree)
{
	m_topActor = getTopActor();
	m_time += deltaTime;
	if (m_time >= 10)
	{
		newGeneration();
	}

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if ((*it)->isDead())
		{
			continue;
		}
		else
		{
			(*it)->update(deltaTime);
			auto objects = quadtree->getObjects((*it)->getBounds());
			auto sensors = (*it)->getSensors();
			for (int i = 0; i < objects.size(); ++i)
			{
				if (auto object = objects[i].lock())
				{
					if (object->hasPolygon())
					{
						if (math::rectIntersectsPolygon((*it)->getPhysicalBounds(), object->getPolygon()))
						{
							(*it)->setDead(true);
							continue;
						}
					}
					else if (object->getBoundingBox().intersects((*it)->getPhysicalBounds()))
					{
						(*it)->setDead(true);
						continue;
					}

					for (int j = 0; j < sensors.size(); ++j)
					{
						if (object->hasPolygon())
						{
							sf::Vector2f a = sensors[j], b = sensors[j];
							bool intersects = math::lineIntersectsPolygon((*it)->getPosition(), sensors[j], a, b, object->getPolygon());
							if (intersects)
							{
								if (a == b || math::distance((*it)->getPosition(), a) < math::distance((*it)->getPosition(), b))
								{
									float value = 1.5f - math::distance<float>((*it)->getPosition(), a)/(*it)->SENSOR_DISTANCE;
									(*it)->setInput(value, j);
								}
								else
								{
									float value = 1.5f - math::distance<float>((*it)->getPosition(), b)/(*it)->SENSOR_DISTANCE;
									(*it)->setInput(value, j);
								}
							}
						}
						else
						{
							bool intersects = false;
							sf::Vector2f point;
							point = math::getLineRectIntersection((*it)->getPosition(), sensors[j], object->getBoundingBox(), intersects);
							if (intersects)
							{
								float value = 1.5f - math::distance<float>((*it)->getPosition(), point)/(*it)->SENSOR_DISTANCE;
								(*it)->setInput(value, j);
							}
						}
					}
				}

				if ((*it)->isDead())
				{
					break;
				}
			}
		}
	}

	if (shouldResetActors())
	{
		newGeneration();
	}
}

void ActorManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it)->draw(target, states);
	}

	/*for (int i = 0; i < 5; ++i)
	{
		if (i < m_actors.size())
		{
			m_actors[i]->draw(target, states);
		}
		else
		{
			continue;
		}
	}*/
}

void ActorManager::drawNeuralNet(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (auto topActor = m_topActor.lock())
	{
		topActor->drawNeuralNet(target, states);
	}
}

void ActorManager::deleteOutsiders(const sf::Rect<int>& bounds)
{
	for (auto it = m_actors.begin(); it != m_actors.end();)
	{
		if (!(*it)->getBounds().intersects(bounds) && !bounds.intersects((*it)->getBounds()))
			it = m_actors.erase(it);
		else
			++it;
	}
}

size_t ActorManager::actorsSize() const
{
	return m_actors.size();
}

bool ActorManager::shouldResetActors() const
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if (!(*it)->isDead())
		{
			std::cout << (*it)->getPosition().x << ':' << (*it)->getPosition().y << std::endl;
			return false;
		}
	}

	return true;
}

void ActorManager::resetActors()
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it).reset(new NNActor());
		(*it)->setPosition(1280/2, 720/2);
	}
}

void ActorManager::resetActors(const std::vector<std::vector<float>> dna)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		//(*it).reset(new NNActor(dna));
		(*it).reset(new NNActor());
		(*it)->setPosition(1280/2, 720/2);
	}
}

void ActorManager::newGeneration()
{
	std::cout << "new gen" << std::endl;
	m_time = 0;
	std::sort(m_actors.begin(), m_actors.end(), ActorCompareDistance());
	auto firstDna = m_actors[0]->getDna(), secondDna = m_actors[1]->getDna();
	auto combinedDna = firstDna;
	int mutatedStrands = combinedDna.size()/10, inheritedStrands = (combinedDna.size()/10)*3;

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		std::cout << '[';
		for (int i = 0; i < combinedDna.size(); ++i)
		{
			int r = std::rand()%100;
			if (r >= 90)
			{
				combinedDna[i] = (double)std::rand()/RAND_MAX;
			}
			else if (r >= 70)
			{
				combinedDna[i] = secondDna[i];
			}

			std::cout << combinedDna[i] << ',';
		}

		std::cout << ']' << std::endl;

		(*it).reset(new NNActor(combinedDna));
		(*it)->setPosition(1280/2, 80);
		combinedDna = firstDna;
	}

	//std::cout << std::endl;
}


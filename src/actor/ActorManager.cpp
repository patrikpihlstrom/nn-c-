#include "ActorManager.hpp"


ActorManager::ActorManager()
{
	m_generations = 0;
	m_space = false;
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
	
	std::sort(m_actors.begin(), m_actors.end(), ActorCompareAge());
	return *m_actors.begin();
}

ActorId ActorManager::addActor(std::shared_ptr<NNActor> actor)
{
	/*else if (auto playerActor = m_playerActor.lock())
	{
		NPCActor* npcActor = dynamic_cast<NPCActor*>(actor.get());
		npcActor->setPlayerActor(m_playerActor);
	}*/
	ActorId id = m_actorIdTracker.addActor();
	actor->assign(id);
	m_actors.push_back(actor);
	return id;
}

void ActorManager::removeActor(const ActorId& id)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			m_actors.erase(it);
			m_actorIdTracker.removeActor(id);
			return;
		}
	}
}

void ActorManager::update(const float& deltaTime, std::shared_ptr<Quadtree> quadtree, Camera& camera)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if (!m_space)
		{
			newGeneration();
			m_space = true;
		}
	}
	else if (m_space)
	{
		m_space = false;
	}

	m_topActor = getTopActor();
	m_time += deltaTime;

	//if (auto actor = m_topActor.lock())
	//{
	//	camera.update(actor->getPosition());
	//}
	//else
	//{
	//	camera.update(m_actors[0]->getPosition());
	//}

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{	
		if ((*it)->getHealth() <= 0)
		{
			(*it)->setDead(true);
		}

		if ((*it)->isDead())
		{
			continue;
		}

		(*it)->update(deltaTime);
		auto objects = quadtree->getObjects((*it)->getBounds());
		auto sensors = (*it)->getSensors();
		for (int i = 0; i < objects.size(); ++i)
		{
			if (auto object = objects[i].lock())
			{
				if (object->dead)
				{
					continue;
				}

				if (object->hasPolygon())
				{
					if (math::rectIntersectsPolygon((*it)->getPhysicalBounds(), object->getPolygon()))
					{
						switch (object->getType())
						{
							case ObjectType::food:
								(*it)->addHealth(500);
								object->dead = true;
							break;

							case ObjectType::obstacle:
								(*it)->setDead(true);
							break;
						}
					}
				}
				else if (object->getBoundingBox().intersects((*it)->getPhysicalBounds()))
				{
					switch (object->getType())
					{
						case ObjectType::food:
							(*it)->addHealth(100);
							object->dead = true;
						break;

						case ObjectType::obstacle:
							(*it)->setDead(true);
						break;
					}
				}

				if ((*it)->isDead())
				{
					continue;
				}

				for (int j = 0; j < sensors.size(); ++j)
				{
					if (object->hasPolygon() && object->getType() == ObjectType::obstacle)
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

					if (!object->hasPolygon() || object->getType() == ObjectType::food)
					{
						bool intersects = false;
						sf::Vector2f point;
						point = math::getLineRectIntersection((*it)->getPosition(), sensors[j], object->getBoundingBox(), intersects);
						if (intersects)
						{
							if (j >= (*it)->SENSOR_COUNT)
							{
								(*it)->setSensor(j, point);
								auto color = object->getColor();
								(*it)->setInput(color.r/255.f, j + (j-(*it)->SENSOR_COUNT)*3);
								(*it)->setInput(color.g/255.f, j + (j-(*it)->SENSOR_COUNT)*3 + 1);
								(*it)->setInput(color.b/255.f, j + (j-(*it)->SENSOR_COUNT)*3 + 2);
							}
							else if (object->getType() != ObjectType::food)
							{
								float value = 1.5f - math::distance<float>((*it)->getPosition(), point)/(*it)->SENSOR_DISTANCE;
								(*it)->setInput(value, j);
							}
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
		(*it)->setPosition(0, 0);
	}
}

void ActorManager::resetActors(const std::vector<double> dna)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it).reset(new NNActor(dna));
		(*it)->setPosition(m_start);
	}
}

void ActorManager::newGeneration()
{
	std::cout << "-- new gen --" << std::endl;
	m_time = 0;
	std::sort(m_actors.begin(), m_actors.end(), ActorCompareAge());

	std::cout << "generation: " << m_generations << std::endl;
	std::cout << "fitness: " << m_actors[0]->getAge() << std::endl;

	auto firstDna = m_actors[0]->getDna(), secondDna = m_actors[1]->getDna();
	auto combinedDna = firstDna;
	int mutatedStrands = combinedDna.size()/10, inheritedStrands = (combinedDna.size()/10)*3;

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		combinedDna = firstDna;
		//std::cout << '[';
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
		}

		(*it).reset(new NNActor(combinedDna));
		(*it)->setDead(false);
		(*it)->setPosition({m_start.x + ((float)std::rand()/RAND_MAX - 0.5f)*100.f, m_start.y + ((float)std::rand()/RAND_MAX - 0.5f)*100.f});
		combinedDna = firstDna;
	}

	m_generations++;
}

void ActorManager::setStart(const sf::Vector2f position)
{
	m_start = position;
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it)->setPosition(position.x, position.y);
	}
}

void ActorManager::setFinish(const math::Polygon polygon)
{
	m_finish = polygon;
}

void ActorManager::processSight(const sf::Image image)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if ((*it)->isDead())
		{
			continue;
		}
		
		for (int i = (*it)->SENSOR_COUNT - 1; i < (*it)->SENSOR_COUNT+(*it)->EYE_COUNT; ++i)
		{
			float r = 0, g = 0, b = 0;
			for (int x = 0, y = 0; x < 5; ++x)
			{
				for (y = 0; y < 5; ++y)
				{
				}
			}
		}
	}
}


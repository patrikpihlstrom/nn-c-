#include "ActorManager.hpp"


ActorManager::ActorManager()
{
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

void ActorManager::update(const float& deltaTime, std::shared_ptr<Quadtree> quadtree)
{
	m_time += deltaTime;
	if (m_time >= 480)
	{
		newGeneration();
	}

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if ((*it)->isDead())
		{
			continue;
		}

		if ((m_time >= 60 && (*it)->getDistance() <= 500) || math::distance<float>((*it)->getPosition(), {0, 0}) >= 1000.f)
		{
			(*it)->setDead(true);
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
					if (object->getBoundingBox().intersects((*it)->getPhysicalBounds()))
					{
						(*it)->setDead(true);
						continue;
					}

					for (int j = 0; j < sensors.size(); ++j)
					{
						bool intersects = false;
						auto point = math::getLineRectIntersection((*it)->getPosition(), sensors[j], object->getBoundingBox(), intersects);
						if (intersects)
						{
							float value = 1.5f - math::distance<float>((*it)->getPosition(), point)/(*it)->SENSOR_DISTANCE;
						(*it)->setInput(value, j);
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
		(*it)->draw(target, states);
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
		(*it)->setPosition(1280/2, 720/2);
	}
}

void ActorManager::resetActors(const std::vector<std::vector<float>> dna)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it).reset(new NNActor(dna));
		(*it)->setPosition(1280/2, 720/2);
	}
}

void ActorManager::newGeneration()
{
	m_time = 0;
	std::sort(m_actors.begin(), m_actors.end(), ActorCompareDistance());
	std::cout << m_actors[0]->getDistance() << std::endl;
	std::vector<std::vector<float>> combinedDna = m_actors[0]->getDna(), secondDna = m_actors[1]->getDna();
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		for (int i = 0; i < combinedDna.size(); ++i)
		{
			int r = std::rand()%100;
			if (r >= 90)
			{
				for (int j = 0; j < combinedDna[i].size(); ++j)
				{
					combinedDna[i][j] = (double)std::rand()/RAND_MAX;
				}
			}
			else if (r >= 50)
			{
				combinedDna[i] = secondDna[i];
			}

			/*std::cout << '[';
			for (int j = 0; j < combinedDna[i].size(); ++j)
			{
				std::cout << combinedDna[i][j] << ", ";
			}
			std::cout << ']';*/
		}

		(*it).reset(new NNActor(combinedDna));
		(*it)->setPosition(1280/2, 720/2);
	}

	//std::cout << std::endl;
}


#include "actor/ActorHolder.hpp"


ActorHolder::ActorHolder()
{
}

ActorHolder::ActorHolder(const sf::Rect<int>& bounds) :
	m_bounds(bounds)
{
}

ActorHolder::~ActorHolder()
{
}

void ActorHolder::setBounds(const sf::Rect<int>& bounds)
{
	m_bounds = bounds;
}

sf::Rect<int> ActorHolder::getBounds() const
{
	return m_bounds;
}

void ActorHolder::addActor(std::shared_ptr<Actor> actor)
{
	if (!getActor(actor->getId()))
		m_actors.push_back(actor);
	else
		std::cout << "Actor: " << actor->getId().id << " already in actor holder." << std::endl;
}

std::vector<std::shared_ptr<Actor>>::iterator ActorHolder::removeActor(const ActorId& actorId)
{
	auto it = m_actors.begin();

	while (it != m_actors.end())
	{
		if ((*it)->getId() == actorId)
			return m_actors.erase(it);
		else
			++it;
	}

	return m_actors.end();
}

void ActorHolder::removeActor(std::vector<std::shared_ptr<Actor>>::iterator actor)
{
	m_actors.erase(actor);
}

std::shared_ptr<Actor> ActorHolder::getActor(const ActorId& actorId) const
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if ((*it)->getId() == actorId)
			return *it;
	}

	return nullptr;
}

bool ActorHolder::isEmpty() const
{
	return m_actors.empty();
}


std::vector<std::weak_ptr<Actor>> ActorHolder::getActors(std::vector<ActorId>& actorIds) const
{
	std::vector<std::weak_ptr<Actor>> actors;
	
	for (int i = 0; i < m_actors.size(); ++i)
	{
		if (std::find(actorIds.begin(), actorIds.end(), m_actors[i]->getId()) == actorIds.end())
		{
			actors.push_back(m_actors[i]);
			actorIds.push_back(m_actors[i]->getId());
		}
	}

	return actors;
}


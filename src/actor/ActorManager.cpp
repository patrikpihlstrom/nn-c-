#include "actor/ActorManager.hpp"


ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
}

void ActorManager::addActor(std::shared_ptr<Actor> actor)
{
	m_actors.push_back(actor);
}

void ActorManager::removeActor(const ActorId& id)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			m_actors.erase(it);
			return;
		}
	}
}

void ActorManager::update(const float& deltaTime, const sf::Rect<int>& bounds)
{
	deleteOutsiders(bounds);

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
		(*it)->update(deltaTime);
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
		if ((*it)->isPlayer())
			continue;

		if (!(*it)->getBounds().intersects(bounds))
			it = m_actors.erase(it);
		else
			++it;
	}
}

size_t ActorManager::actorsSize() const
{
	return m_actors.size();
}


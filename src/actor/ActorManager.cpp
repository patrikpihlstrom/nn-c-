#include "actor/ActorManager.hpp"


ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
}

std::weak_ptr<Actor> ActorManager::getActor(const ActorId& id) const
{
	for (int i = 0; i < m_actors.size(); ++i)
	{
		if (m_actors[i]->getId() == id)
			return m_actors[i];
	}

	return {};
}

void ActorManager::addActor(std::shared_ptr<Actor> actor)
{
	if (actor->isPlayer())
		m_playerActor = actor;
	else if (auto playerActor = m_playerActor.lock())
	{
		NPCActor* npcActor = dynamic_cast<NPCActor*>(actor.get());
		npcActor->setPlayerActor(m_playerActor);
	}

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
	deleteOutsiders({bounds.left - bounds.width*2, bounds.top - bounds.height*2, bounds.width*4, bounds.height*4});

	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it)->update(deltaTime);
		if ((*it)->isPlayer())
			continue;

		for (auto iter = m_actors.begin(); iter != m_actors.end(); ++iter)
		{
			if ((*iter)->isPlayer() || (*iter)->getId().id == (*it)->getId().id || math::distance<float>((*it)->getPosition(), (*iter)->getPosition()) >= 16)
				continue;

			if ((*it)->getPosition().x < (*iter)->getPosition().x)
			{
				(*it)->move(-1, 0);
				(*iter)->move(1, 0);
			}
			else if ((*it)->getPosition().x < (*iter)->getPosition().x)
			{
				(*it)->move(1, 0);
				(*iter)->move(-1, 0);
			}

			if ((*it)->getPosition().y < (*iter)->getPosition().y)
			{
				(*it)->move(0, -1);
				(*iter)->move(0, 1);
			}
			else if ((*it)->getPosition().x < (*iter)->getPosition().x)
			{
				(*it)->move(0, 1);
				(*iter)->move(0, -1);
			}
		}
	}

	std::sort(m_actors.begin(), m_actors.end(), ActorCompare());
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
		{
			++it;
			continue;
		}

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


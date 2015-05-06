#include "actor/ActorManager.hpp"


ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
}

void ActorManager::createNewPlayerActor(const PlayerActor& playerActor)
{
	createHolders(playerActor.getBounds());
	auto holders = getHolders(playerActor.getBounds());

	if (!holders.empty())
	{
		std::shared_ptr<PlayerActor> actor(new PlayerActor(playerActor));
		
		for (int i = 0; i < holders.size(); ++i)
		{
			if (auto holder = holders[i].lock())
				holder->addActor(actor);
		}
	}
}

void ActorManager::createNewNPCActor(const NPCActor& npcActor)
{
	createHolders(npcActor.getBounds());
	auto holders = getHolders(npcActor.getBounds());

	if (!holders.empty())
	{
		std::shared_ptr<NPCActor> actor(new NPCActor(npcActor));
		
		for (int i = 0; i < holders.size(); ++i)
		{
			if (auto holder = holders[i].lock())
				holder->addActor(actor);
		}
	}
}

void ActorManager::removeActor(const ActorId& id)
{
	for (auto it = m_actorHolders.begin(); it != m_actorHolders.end(); ++it)
		it->second->removeActor(id);
}

void ActorManager::removeActor(const ActorId& id, const sf::Rect<int>& bounds)
{
	auto holders = getHolders(bounds);

	for (int i = 0; i < holders.size(); ++i)
	{
		if (auto holder = holders[i].lock())
		{
			holder->removeActor(id);
		}
	}
}

void ActorManager::createHolders(const sf::Rect<int>& bounds)
{
	int x = (int)(bounds.left/ACTOR_HOLDER_SIZE);
	int y = (int)(bounds.top/ACTOR_HOLDER_SIZE);
	int width = (int)(bounds.width/ACTOR_HOLDER_SIZE);
	int height = (int)(bounds.height/ACTOR_HOLDER_SIZE);

	for (int i = x, j = y; i <= x + width; ++i)
	{
		for (j = y; j <= y + height; ++j)
		{
			if (m_actorHolders.find({i, j}) == m_actorHolders.end())
			{
				ActorHolder actorHolder{{i*ACTOR_HOLDER_SIZE, j*ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE}};
				m_actorHolders[{i, j}].reset(new ActorHolder(actorHolder));
			}
		}
	}
}

std::vector<std::weak_ptr<ActorHolder>> ActorManager::getHolders(const sf::Rect<int>& bounds) const
{
	std::vector<std::weak_ptr<ActorHolder>> holders;
	holders.reserve(4);

	// Only works for objects smaller than 1024*1024
	auto it = m_actorHolders.find({(int)(bounds.left/ACTOR_HOLDER_SIZE), (int)(bounds.top/ACTOR_HOLDER_SIZE)});
	if (it != m_actorHolders.end())
		holders.push_back(it->second);

	it = m_actorHolders.find({(int)((bounds.left + bounds.width)/ACTOR_HOLDER_SIZE), (int)(bounds.top/ACTOR_HOLDER_SIZE)});
	if (it != m_actorHolders.end())
		holders.push_back(it->second);

	it = m_actorHolders.find({(int)(bounds.left/ACTOR_HOLDER_SIZE), (int)((bounds.top + bounds.height)/ACTOR_HOLDER_SIZE)});
	if (it != m_actorHolders.end())
		holders.push_back(it->second);

	it = m_actorHolders.find({(int)((bounds.left + bounds.width)/ACTOR_HOLDER_SIZE), (int)((bounds.top + bounds.height)/ACTOR_HOLDER_SIZE)});
	if (it != m_actorHolders.end())
		holders.push_back(it->second);

	return holders;
}

void ActorManager::update(const float& deltaTime)
{
	auto actors = getUniqueActors();

	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		if (auto actor = it->lock())
			actor->update(deltaTime);
	}
}

std::vector<std::weak_ptr<Actor>> ActorManager::getUniqueActors() const
{
	std::vector<ActorId> actorIds;
	std::vector<std::weak_ptr<Actor>> actors;

	for (auto it = m_actorHolders.begin(); it != m_actorHolders.end(); ++it)
	{
		auto holderActors = it->second->getActors(actorIds);
		actors.reserve(holderActors.size());
		actors.insert(actors.end(), holderActors.begin(), holderActors.end());
	}

	return actors;
}

std::vector<std::weak_ptr<Actor>> ActorManager::getUniqueActors(const sf::Rect<int>& bounds) const
{
	std::vector<ActorId> actorIds;
	std::vector<std::weak_ptr<Actor>> actors;
	auto holders = getHolders(bounds);

	for (auto it = holders.begin(); it != holders.end(); ++it)
	{
		if (auto holder = it->lock())
		{
			auto holderActors = holder->getActors(actorIds);
			actors.reserve(holderActors.size());
			actors.insert(actors.end(), holderActors.begin(), holderActors.end());
		}
	}

	return actors;
}

void ActorManager::draw(const sf::Rect<int>& bounds, sf::RenderTarget& target, sf::RenderStates states) const
{
	auto actors = getUniqueActors(bounds);

	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		if (auto actor = it->lock())
			actor->draw(target, states);
	}
}

std::weak_ptr<Actor> ActorManager::getActor(const ActorId& id) const
{
	for (auto it = m_actorHolders.begin(); it != m_actorHolders.end(); ++it)
	{
		auto actor = it->second->getActor(id);

		if (actor)
			return actor;
	}

	return {};
}

std::weak_ptr<Actor> ActorManager::getActor(const ActorId& id, const sf::Rect<int>& bounds) const
{
	auto actorHolders = getHolders(bounds);
	for (auto it = actorHolders.begin(); it != actorHolders.end(); ++it)
	{
		if (auto holder = it->lock())
		{
			auto actor = holder->getActor(id);

			if (actor)
				return actor;
		}
	}

	return {};
}


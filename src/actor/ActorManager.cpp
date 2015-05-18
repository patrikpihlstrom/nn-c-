#include "actor/ActorManager.hpp"


ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
}

void ActorManager::addActor(std::shared_ptr<Actor> actor)
{
	auto holders = getHolders(actor->getBounds());

	if (!holders.empty())
	{
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
	int _x = (int)((bounds.left + bounds.width)/ACTOR_HOLDER_SIZE);
	int y = (int)(bounds.top/ACTOR_HOLDER_SIZE);
	int _y = (int)((bounds.top + bounds.height)/ACTOR_HOLDER_SIZE);

	if (bounds.left < 0)
		x--;

	if (bounds.top < 0)
		y--;

	if (bounds.left + bounds.width < 0)
		_x--;

	if (bounds.top + bounds.height < 0)
		_y--;

	if (bounds.width >= ACTOR_HOLDER_SIZE || bounds.height >= ACTOR_HOLDER_SIZE)
	{
		int width = (int)(bounds.width/(ACTOR_HOLDER_SIZE/2));
		int height = (int)(bounds.height/(ACTOR_HOLDER_SIZE/2));

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

	if (m_actorHolders.find({x, y}) == m_actorHolders.end())
	{
		ActorHolder actorHolder{{x*ACTOR_HOLDER_SIZE, y*ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE}};
		m_actorHolders[{x, y}].reset(new ActorHolder(actorHolder));
	}

	if (m_actorHolders.find({_x, y}) == m_actorHolders.end())
	{
		ActorHolder actorHolder{{_x*ACTOR_HOLDER_SIZE, y*ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE}};
		m_actorHolders[{_x, y}].reset(new ActorHolder(actorHolder));
	}
	
	if (m_actorHolders.find({x, _y}) == m_actorHolders.end())
	{
		ActorHolder actorHolder{{x*ACTOR_HOLDER_SIZE, _y*ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE}};
		m_actorHolders[{x, _y}].reset(new ActorHolder(actorHolder));
	}
	
	if (m_actorHolders.find({_x, _y}) == m_actorHolders.end())
	{
		ActorHolder actorHolder{{_x*ACTOR_HOLDER_SIZE, _y*ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE}};
		m_actorHolders[{_x, _y}].reset(new ActorHolder(actorHolder));
	}
}

std::vector<std::weak_ptr<ActorHolder>> ActorManager::getHolders(const sf::Rect<int>& bounds)
{
	createHolders(bounds);
	return getHoldersConst(bounds);
}

std::vector<std::weak_ptr<ActorHolder>> ActorManager::getHoldersConst(const sf::Rect<int>& bounds) const
{
	std::vector<std::weak_ptr<ActorHolder>> holders;

	int x = (int)(bounds.left/ACTOR_HOLDER_SIZE);
	int _x = (int)((bounds.left + bounds.width)/ACTOR_HOLDER_SIZE);
	int y = (int)(bounds.top/ACTOR_HOLDER_SIZE);
	int _y = (int)((bounds.top + bounds.height)/ACTOR_HOLDER_SIZE);

	if (bounds.left < 0)
		x--;

	if (bounds.top < 0)
		y--;

	if (bounds.left + bounds.width < 0)
		_x--;

	if (bounds.top + bounds.height < 0)
		_y--;

	if (bounds.width >= ACTOR_HOLDER_SIZE || bounds.height >= ACTOR_HOLDER_SIZE)
	{
		int width = (int)(bounds.width/(ACTOR_HOLDER_SIZE)) + 1;
		int height = (int)(bounds.height/(ACTOR_HOLDER_SIZE)) + 1;

		for (int i = x, j = y; i <= x + width; ++i)
		{
			for (j = y; j <= y + height; ++j)
			{
				auto holder = m_actorHolders.find({i, j});
				if (holder != m_actorHolders.end())
					holders.push_back(holder->second);
			}
		}
	}
	else
	{
		auto holder = m_actorHolders.find({x, y});
		if (holder != m_actorHolders.end())
			holders.push_back(holder->second);

		holder = m_actorHolders.find({_x, y});
		if (holder != m_actorHolders.end())
			holders.push_back(holder->second);

		holder = m_actorHolders.find({x, _y});
		if (holder != m_actorHolders.end())
			holders.push_back(holder->second);

		holder = m_actorHolders.find({_x, _y});
		if (holder != m_actorHolders.end())
			holders.push_back(holder->second);
	}

	return holders;
}

void ActorManager::update(const float& deltaTime, const sf::Rect<int>& bounds)
{
	auto actors = getUniqueActors(bounds);

	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		if (auto actor = it->lock())
		{
			auto preHolders = getHolders(actor->getBounds());
			actor->update(deltaTime);
			auto postHolders = getHolders(actor->getBounds());

			for (auto it = preHolders.begin(); it != preHolders.end(); ++it)
			{
				if (auto preHolder = it->lock())
				{
					bool found = false;
					for (auto iter = postHolders.begin(); iter != postHolders.end(); ++iter)
					{
						if (auto postHolder = iter->lock())
						{
							if (preHolder->getBounds() == postHolder->getBounds())
								found = true;
						}
					}

					if (!found)
					{
						preHolder->removeActor(actor->getId());
					}
				}
			}

			for (auto iter = postHolders.begin(); iter != postHolders.end(); ++iter)
			{
				if (auto postHolder = iter->lock())
					postHolder->addActor(actor);
			}
		}
	}

	for (auto it = m_actorHolders.begin(); it != m_actorHolders.end();)
	{
		if (it->second->isEmpty())
			it = m_actorHolders.erase(it);
		else
			++it;
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
	auto holders = getHoldersConst(bounds);
	std::vector<ActorId> actorIds;
	std::vector<std::weak_ptr<Actor>> actors;

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

	for (auto it = m_actorHolders.begin(); it != m_actorHolders.end(); ++it)
	{
		sf::RectangleShape rect;
		rect.setPosition(it->first.x*ACTOR_HOLDER_SIZE, it->first.y*ACTOR_HOLDER_SIZE);
		rect.setSize(sf::Vector2f(ACTOR_HOLDER_SIZE, ACTOR_HOLDER_SIZE));
		rect.setFillColor(sf::Color(255, 0, 0, 10));//it->second->size()*10 > 255 ? 255:it->second->size()*10));
		rect.setOutlineColor(sf::Color(255, 0, 0, 0));
		rect.setOutlineThickness(1.f);
		target.draw(rect, states);
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
	auto actorHolders = getHoldersConst(bounds);
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


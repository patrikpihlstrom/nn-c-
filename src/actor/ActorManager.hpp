#pragma once

#include <memory>
#include <unordered_map>

#include "actor/PlayerActor.hpp"
#include "actor/NPCActor.hpp"
#include "actor/ActorHolder.hpp"

const uint16_t ACTOR_HOLDER_SIZE = 512;

class ActorManager
{
public:
	ActorManager();
	~ActorManager();

	void addActor(std::shared_ptr<Actor> actor);

	void removeActor(const ActorId& id);
	void removeActor(const ActorId& id, const sf::Rect<int>& bounds);

	void createHolders(const sf::Rect<int>& bounds);
	std::vector<std::weak_ptr<ActorHolder>> getHolders(const sf::Rect<int>& bounds);
	std::vector<std::weak_ptr<ActorHolder>> getHoldersConst(const sf::Rect<int>& bounds) const;

	void update(const float& deltaTime, const sf::Rect<int>& bounds);
	void draw(const sf::Rect<int>& bounds, sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<std::weak_ptr<Actor>> getUniqueActors() const;
	std::vector<std::weak_ptr<Actor>> getUniqueActors(const sf::Rect<int>& bounds) const;

	std::weak_ptr<Actor> getActor(const ActorId& id) const;
	std::weak_ptr<Actor> getActor(const ActorId& id, const sf::Rect<int>& bounds) const;

private:
	struct Key
	{
		int x, y;

		bool operator==(const Key& key) const
		{
			return x == key.x && y == key.y;
		}
	};

	struct Hasher
	{
		std::size_t operator()(const Key& key) const
		{
			int hash = 17;
			hash = ((hash + key.x) << 5) - (hash + key.x);
			hash = ((hash + key.y) << 5) - (hash + key.y);
			return hash;
		}
	};

	std::unordered_map<Key, std::shared_ptr<ActorHolder>, Hasher> m_actorHolders;
};


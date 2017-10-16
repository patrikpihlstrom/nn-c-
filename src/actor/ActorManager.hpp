#pragma once

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "PlayerActor.hpp"
#include "ActorIdTracker.hpp"
#include "NNActor.hpp"
#include "Actor.hpp"
#include "../world/Quadtree.hpp"

class ActorManager : public sf::Drawable
{
public:
	ActorManager();
	~ActorManager();

	ActorId addActor(std::shared_ptr<Actor> actor);
	void removeActor(const ActorId& id);

	void update(const float& deltaTime, std::shared_ptr<Quadtree> quadtree);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::weak_ptr<Actor> getActor(const ActorId& id) const;

	size_t actorsSize() const;

private:
	void deleteOutsiders(const sf::Rect<int>& bounds);
	ActorIdTracker m_actorIdTracker;
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::weak_ptr<Actor> m_playerActor;

	struct ActorCompare
	{
		bool operator()(const std::shared_ptr<Actor> l, const std::shared_ptr<Actor> r)
		{
			return *l < *r;
		}
	};
};


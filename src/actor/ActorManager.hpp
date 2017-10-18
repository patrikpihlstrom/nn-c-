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

	ActorId addActor(std::shared_ptr<NNActor> actor);
	void removeActor(const ActorId& id);

	void update(const float& deltaTime, std::shared_ptr<Quadtree> quadtree);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::weak_ptr<NNActor> getActor(const ActorId& id) const;

	size_t actorsSize() const;

private:
	void deleteOutsiders(const sf::Rect<int>& bounds);
	ActorIdTracker m_actorIdTracker;
	std::vector<std::shared_ptr<NNActor>> m_actors;
	std::weak_ptr<Actor> m_playerActor;
	float m_time;

	bool shouldResetActors() const;
	void resetActors();
	void resetActors(const std::vector<std::vector<float>> dna);

	void newGeneration();

	struct ActorCompare
	{
		bool operator()(const std::shared_ptr<NNActor> l, const std::shared_ptr<NNActor> r)
		{
			return *l < *r;
		}
	};

	struct ActorCompareDistance
	{
		bool operator()(const std::shared_ptr<NNActor> l, const std::shared_ptr<NNActor> r)
		{
			return l->getDistance() > r->getDistance();
		}
	};
};


#pragma once

#include <memory>
#include <iostream>

#include "actor/Actor.hpp"


class ActorHolder
{
public:
	ActorHolder();
	ActorHolder(const sf::Rect<int>& bounds);
	~ActorHolder();

	void setBounds(const sf::Rect<int>& bounds);
	sf::Rect<int> getBounds() const;

	void addActor(std::shared_ptr<Actor> actor);

	void removeActor(std::vector<std::shared_ptr<Actor>>::iterator actor);
	std::vector<std::shared_ptr<Actor>>::iterator removeActor(const ActorId& actorId);

	std::shared_ptr<Actor> getActor(const ActorId& actorId) const;

	bool isEmpty() const;

	std::vector<std::weak_ptr<Actor>> getActors(std::vector<ActorId>& actorIds) const;

private:
	std::vector<std::shared_ptr<Actor>> m_actors;

	sf::Rect<int> m_bounds;
};


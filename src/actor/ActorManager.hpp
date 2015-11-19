#pragma once

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "actor/PlayerActor.hpp"
#include "actor/NPCActor.hpp"

class ActorManager : public sf::Drawable
{
public:
	ActorManager();
	~ActorManager();

	void addActor(std::shared_ptr<Actor> actor);
	void removeActor(const ActorId& id);

	void update(const float& deltaTime, const sf::Rect<int>& bounds);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	std::weak_ptr<Actor> getActor(const ActorId& id) const;

	size_t actorsSize() const;

private:
	void deleteOutsiders(const sf::Rect<int>& bounds);
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


#pragma once

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "ActorIdTracker.hpp"
#include "NNActor.hpp"
#include "../world/Quadtree.hpp"
#include "../application/Camera.hpp"

class ActorManager : public sf::Drawable
{
public:
	ActorManager();
	~ActorManager();

	ActorId addActor(std::shared_ptr<NNActor> actor);
	void removeActor(const ActorId& id);

	void setStart(const sf::Vector2f position);
	void setFinish(const math::Polygon polygon);
	void update(const float& deltaTime, std::shared_ptr<Quadtree> quadtree, Camera& camera);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawNeuralNet(sf::RenderTarget& target, sf::RenderStates states) const;

	std::weak_ptr<NNActor> getActor(const ActorId& id) const;
	std::weak_ptr<NNActor> getTopActor();

	size_t actorsSize() const;

	void processSight(const sf::Image image);

private:
	void deleteOutsiders(const sf::Rect<int>& bounds);
	ActorIdTracker m_actorIdTracker;

	std::vector<std::shared_ptr<NNActor>> m_actors;
	std::weak_ptr<NNActor> m_topActor;

	sf::Vector2f m_start;
	math::Polygon m_finish;

	float m_time;
	unsigned int m_generations;
	bool m_space;

	bool shouldResetActors() const;
	void resetActors();
	void resetActors(const std::vector<double> dna);

	void newGeneration();

	struct ActorCompare
	{
		bool operator()(const std::shared_ptr<NNActor> l, const std::shared_ptr<NNActor> r)
		{
			return *l < *r;
		}
	};

	struct ActorCompareAge
	{
		bool operator()(const std::shared_ptr<NNActor> l, const std::shared_ptr<NNActor> r)
		{
			return l->getAge() > r->getAge();
		}
	};
};


#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "../actor/NNActor.hpp"
#include "../actor/ActorManager.hpp"
#include "../actor/ActorIdTracker.hpp"
#include "../application/TextureHolder.hpp"
#include "../application/Camera.hpp"
#include "../object/ObjectIdTracker.hpp"
#include "../object/Object.hpp"
#include "Quadtree.hpp"


class World : public sf::Drawable
{
public:
	World();
	World(const long& seed);
	~World();

	void load(const std::string& path);

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::View getView() const;

private:
	long m_seed;

	float m_foodTimer, m_sightTimer;

	struct Hash
	{
		std::size_t operator()(const sf::Vector2i& key) const
		{
			int hash = 51;
			hash = ((hash + key.x) << 5) - (hash + key.x);
			hash = ((hash + key.y) << 5) - (hash + key.y);
			return hash;
		}
	};

	std::vector<int> getAllInstancesOfChar(const std::string& string, const char& character)
	{
		std::vector<int> indices;
		for (int i = 0; i < string.size(); ++i)
		{
			if (string[i] == character)
				indices.push_back(i);
		}

		return indices;
	}

	std::shared_ptr<Quadtree> m_quadtree;
	std::vector<std::shared_ptr<Object>> m_objects;
	ObjectIdTracker m_objectIdTracker;
	ActorManager m_actorManager;

	ObjectType getType(const std::string typeString)
	{
		if (typeString.size() >= 5 && typeString.substr(0, 5) == "start")
		{
			return ObjectType::start;
		}
		else if (typeString.size() >= 6 && typeString.substr(0, 6) == "finish")
		{
			return ObjectType::finish;
		}

		return ObjectType::obstacle;
	}

	Camera m_camera;
};


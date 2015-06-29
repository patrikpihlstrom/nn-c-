#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"
#include "actor/NPCSpawner.hpp"
#include "world/Quadtree.hpp"
#include "world/procedural/Noise.hpp"
#include "world/procedural/RockGenerator.hpp"
#include "world/procedural/PathGenerator.hpp"
#include "world/Chunk.hpp"
#include "application/TextureHolder.hpp"
#include "application/Camera.hpp"
#include "object/ObjectIdTracker.hpp"
#include "object/Object.hpp"


class World : public sf::Drawable
{
public:
	World(const long& seed);
	~World();

	void update(const float& deltaTime, const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::weak_ptr<Quadtree> getQuadtree() const;
	std::weak_ptr<ActorManager> getActorManager() const;

	Camera getCamera() const;
	
private:
	bool m_zooming;

	void addObject(const Object& object);

	std::shared_ptr<Quadtree> m_quadtree;
	std::shared_ptr<ActorManager> m_actorManager;
	std::shared_ptr<NPCSpawner> m_npcSpawner;
	std::unique_ptr<RockGenerator> m_rockGenerator;
	std::unique_ptr<PathGenerator> m_pathGenerator;
	std::unique_ptr<TextureHolder> m_textureHolder;
	std::unique_ptr<ObjectIdTracker> m_objectIdTracker;
	std::unique_ptr<ActorIdTracker> m_actorIdTracker;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Noise> m_noise;
	
	std::shared_ptr<PlayerActor> m_playerActor;

	void addChunks(const sf::Vector2f& position); // Add chunks close to the player's position.
	void addChunk(const sf::Vector2i& position); // Called by World::addChunks(position).

	void removeChunks(const sf::Vector2f& position); // Remove chunks far from the player's position.

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

	long m_seed;

	std::unordered_map<sf::Vector2i, std::shared_ptr<Chunk>, Hash> m_chunks;
};


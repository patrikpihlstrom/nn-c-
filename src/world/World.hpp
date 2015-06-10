#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"
#include "actor/NPCSpawner.hpp"
#include "world/Quadtree.hpp"
#include "world/procedural/RockGenerator.hpp"
#include "world/procedural/PathGenerator.hpp"
#include "application/TextureHolder.hpp"
#include "application/Camera.hpp"
#include "object/ObjectIdTracker.hpp"
#include "object/Object.hpp"


class World : public sf::Drawable
{
public:
	World();
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

	std::vector<Path> m_paths;

	sf::Sprite m_sprite;
};


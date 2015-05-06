#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"
#include "map/Quadtree.hpp"
#include "map/procedural/RockGenerator.hpp"
#include "map/TextureHolder.hpp"
#include "application/Camera.hpp"
#include "object/ObjectIdTracker.hpp"
#include "object/GameObject.hpp"
#include "object/ShadowUpdater.hpp"
#include "object/Decal.hpp"


class Map : public sf::Drawable
{
public:
	Map();
	~Map();

	void update(const float& deltaTime, const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::weak_ptr<Quadtree> getQuadtree() const;
	std::weak_ptr<ActorManager> getActorManager() const;

	Camera getCamera() const;
	
private:
	void addObject(GameObject& object);
	void addDecal(Decal& decal);

	bool space;

	std::shared_ptr<Quadtree> m_quadtree;
	std::shared_ptr<ActorManager> m_actorManager;
	std::unique_ptr<RockGenerator> m_rockGenerator;
	std::unique_ptr<TextureHolder> m_textureHolder;
	std::unique_ptr<ObjectIdTracker> m_objectIdTracker;
	std::unique_ptr<ActorIdTracker> m_actorIdTracker;
	std::unique_ptr<ShadowUpdater> m_shadowUpdater;
	std::unique_ptr<Camera> m_camera;
	std::vector<std::weak_ptr<Object>> m_gameObjects;
	std::vector<std::weak_ptr<Object>> m_decals;
	std::weak_ptr<Object> m_light;
};


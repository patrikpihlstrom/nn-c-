#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "map/Quadtree.hpp"
#include "map/procedural/RockGenerator.hpp"
#include "application/TextureHolder.hpp"
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

	void update(const sf::RenderWindow& window, const Camera& camera);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::weak_ptr<Quadtree> getQuadtree() const;
	
private:
	void addObject(GameObject& object);
	void addDecal(Decal& decal);

	bool space;

	std::shared_ptr<Quadtree> m_quadtree;
	std::vector<std::weak_ptr<Object>> m_gameObjects;
	std::vector<std::weak_ptr<Object>> m_decals;
	std::unique_ptr<TextureHolder> m_textureHolder;
	std::unique_ptr<ObjectIdTracker> m_objectIdTracker;
	std::unique_ptr<ShadowUpdater> m_shadowUpdater;
	std::weak_ptr<Object> m_light;
	std::unique_ptr<RockGenerator> m_rockGenerator;
};


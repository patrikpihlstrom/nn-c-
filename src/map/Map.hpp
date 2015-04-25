#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "Quadtree.hpp"
#include "../application/TextureHolder.hpp"
#include "../application/Camera.hpp"
#include "../object/ObjectIdTracker.hpp"
#include "../object/GameObject.hpp"
#include "../object/ShadowUpdater.hpp"


class Map : public sf::Drawable
{
public:
	Map();
	~Map();

	void update(const sf::RenderWindow& window, const Camera& camera);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void load(const std::string& file);
	void save() const;

	std::weak_ptr<Quadtree> getQuadtree() const;

private:
	void addObject(GameObject& object, const std::string& mtl = "(null)");

	std::shared_ptr<Quadtree> m_quadtree;
	std::vector<std::shared_ptr<Object>> m_objects;
	std::unique_ptr<TextureHolder> m_textureHolder;
	std::unique_ptr<ObjectIdTracker> m_objectIdTracker;
	std::unique_ptr<ShadowUpdater> m_shadowUpdater;
	std::weak_ptr<Object> m_light;
};


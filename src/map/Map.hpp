#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "Quadtree.hpp"
#include "../application/TextureHolder.hpp"


class Map : public sf::Drawable
{
public:
	Map();
	~Map();

	void addPolygon(const math::Polygon& polygon, const std::string& mtl = "");
	void removePolygon(std::shared_ptr<math::Polygon> polygon);

	std::shared_ptr<math::Polygon> getPolygon(const sf::Vector2f& position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void load(const std::string& file);
	void save() const;

	std::weak_ptr<Quadtree> getQuadtree() const;

private:
	unsigned int m_index;
	std::shared_ptr<Quadtree> m_quadtree;
	std::unique_ptr<TextureHolder> m_textureHolder;
	std::map<unsigned int, sf::ConvexShape> m_polygons;
};


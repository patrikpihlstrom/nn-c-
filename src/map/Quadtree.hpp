#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "../application/Math.hpp"
#include "../entity/JumpCheck.hpp"


const unsigned char MAX_LEVEL = 3;

class Quadtree : public sf::Drawable
{
public:
	Quadtree();
	Quadtree(const sf::Vector2f& position, const sf::Vector2f& dimensions, const bool hasChildren = false, const unsigned char level = 0);
	~Quadtree();

	void update();

	void insert(std::shared_ptr<math::Polygon> polygon, unsigned char index);
	bool remove(unsigned char index);

	bool canMerge() const; // Checks itself and its siblings.
	bool canMergeChildren() const;
	bool empty() const;
	bool hasChildren() const;

	void save(std::ofstream& file, std::vector<unsigned char>& saved) const;

	math::Polygon polygon;

	std::shared_ptr<math::Polygon> getPolygon(const sf::Vector2f& position);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void getQuadtrees(std::vector<std::weak_ptr<Quadtree>>& quadtrees, const math::Polygon& polygon) const;
	std::vector<std::shared_ptr<math::Polygon>> getPolygons(std::vector<unsigned char>& indices) const;

	sf::Vector2f checkCollisions(const math::Polygon& polygon, JumpCheck& jumpCheckLeft, JumpCheck& jumpCheckBottom, JumpCheck& jumpCheckRight, const sf::Vector2f& velocity, std::vector<unsigned int>& indices) const;

private:
	std::shared_ptr<Quadtree> m_children[4];
	std::weak_ptr<Quadtree> m_parent;
	std::map<unsigned char, std::shared_ptr<math::Polygon>> m_polygons;

	void split();
	void mergeChildren();

	unsigned char m_level;

	sf::Vector2f m_position, m_dimensions;

	bool m_hasChildren;
};


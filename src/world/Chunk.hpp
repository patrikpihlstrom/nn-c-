#pragma once

#include <SFML/Graphics.hpp>

#include "world/Quadtree.hpp"

#ifdef _WIN32
#include <sys\types.h>
#endif

const static int CHUNK_SIZE = 256;

class Chunk : public sf::Drawable, public sf::Transformable
{
public:
	Chunk();
	~Chunk();
	
	void build();

	std::weak_ptr<Quadtree> getQuadtree() const;

	sf::Color getColor() const;
	void setColor(const sf::Color color);
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Color m_color;
	std::shared_ptr<Quadtree> m_quadtree;

};


#pragma once

#include <SFML/Graphics.hpp>

#ifdef _WIN32 || _WIN64
#include <sys\types.h>
#endif

const static int CHUNK_SIZE = 256;

class Chunk : public sf::Drawable, public sf::Transformable
{
public:
	Chunk();
	~Chunk();

	sf::Color getColor() const;
	void setColor(const sf::Color color);

	void build();
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Color m_color;

};


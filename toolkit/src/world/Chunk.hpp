#pragma once

#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <sys\types.h>
#endif

enum class ChunkType
{
	Barren,
	Town,
	Crossroads
};

const static int CHUNK_SIZE = 1024;

class Chunk : public sf::Drawable
{
public:
	Chunk();
	~Chunk();
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getPosition() const;
	void setPosition(const sf::Vector2i& position);

private:
	sf::Vector2i m_position;
};


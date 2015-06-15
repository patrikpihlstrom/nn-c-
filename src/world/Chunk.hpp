#pragma once

#include <SFML/Graphics.hpp>

const static uint16_t CHUNK_SIZE = 64;

class Chunk : public sf::Drawable, public sf::Transformable
{
public:
	Chunk();
	~Chunk();

	sf::Color getColor() const;
	void setColor(const sf::Color color);
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Color m_color;

};


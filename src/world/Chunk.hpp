#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <sys\types.h>
#endif


const static int CHUNK_SIZE = 1024;

class Chunk : public sf::Drawable
{
public:
	Chunk();
	Chunk(const sf::Vector2i& position);
	~Chunk();
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getPosition() const;
	void setPosition(const sf::Vector2i& position);

	int getWeight() const;
	void setWeight(const int& weight);

	sf::Color getColor() const;
	void setColor(const sf::Color& color);

private:
	sf::Vector2i m_position;
	int m_weight;
	sf::Color m_color;
};


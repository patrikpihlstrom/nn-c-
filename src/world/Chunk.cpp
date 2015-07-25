#include "world/Chunk.hpp"


Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rectShape;
	rectShape.setSize(sf::Vector2f(CHUNK_SIZE, CHUNK_SIZE));
	rectShape.setPosition((sf::Vector2f)m_position);
	target.draw(rectShape, states);
}

sf::Vector2i Chunk::getPosition() const
{
	return m_position;
}

void Chunk::setPosition(const sf::Vector2i& position)
{
	m_position = position;
}


#include "world/Chunk.hpp"


Chunk::Chunk()
{
}

Chunk::Chunk(const sf::Vector2i& position) :
	m_position(position)
{
}

Chunk::~Chunk()
{
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

sf::Vector2i Chunk::getPosition() const
{
	return m_position;
}

void Chunk::setPosition(const sf::Vector2i& position)
{
	m_position = position;
}


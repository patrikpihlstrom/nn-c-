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
	sf::RectangleShape rectShape;
	rectShape.setFillColor(sf::Color(255*((float)m_weight/100), 0, 0, 100));
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

int Chunk::getWeight() const
{
	return m_weight;
}

void Chunk::setWeight(const int& weight)
{
	m_weight = (int)weight;
}


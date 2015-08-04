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
	rectShape.setFillColor(m_color);
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

sf::Color Chunk::getColor() const
{
	return m_color;
}

void Chunk::setColor(const sf::Color& color)
{
	m_color = color;
	m_color.r *= (float)m_weight/100;
	m_color.g *= (float)m_weight/100;
	m_color.b *= (float)m_weight/100;
}


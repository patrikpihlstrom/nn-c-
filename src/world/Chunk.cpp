#include "world/Chunk.hpp"


Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

void Chunk::build(const double noise)
{
	setColor(sf::Color(255*noise, 255*noise, 255*noise));
}

std::weak_ptr<Quadtree> Chunk::getQuadtree() const
{
	return m_quadtree;
}

sf::Color Chunk::getColor() const
{
	return m_color;
}

void Chunk::setColor(const sf::Color color)
{
	m_color = color;
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rectShape;
	rectShape.setSize(sf::Vector2f(CHUNK_SIZE, CHUNK_SIZE));
	rectShape.setPosition(getPosition());
	rectShape.setFillColor(m_color);

	target.draw(rectShape, states);
}


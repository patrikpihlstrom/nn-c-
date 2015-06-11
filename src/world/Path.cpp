#include "world/Path.hpp"


Path::Path()
{
	m_vertexArray.setPrimitiveType(sf::PrimitiveType::LinesStrip);
}

Path::~Path()
{
}

void Path::setPoint(const uint8_t& index, const sf::Vector2f& point)
{
	if (index > m_vertexArray.getVertexCount())
	{
		std::cout << "Index out of bounds: " << index << std::endl << "Size of vertex array is: " << m_vertexArray.getVertexCount() << std::endl << std::endl;
		return;
	}

	m_vertexArray[index] = {point, sf::Color(255, 0, 0)};
}

sf::Vector2f Path::getPoint(const uint8_t& index) const
{
	return m_vertexArray[index].position;
}

void Path::addPoint(const sf::Vector2f& point)
{
	m_vertexArray.append({point, sf::Color(255, 0, 0)});
}

sf::Vector2f Path::getFirst() const
{
	if (m_vertexArray.getVertexCount() != 0)
		return m_vertexArray[0].position;
	else
		return {0, 0};
}

sf::Vector2f Path::getLast() const
{
	if (m_vertexArray.getVertexCount() != 0)
		return m_vertexArray[m_vertexArray.getVertexCount() - 1].position;
	else
		return {0, 0};
}

void Path::build(const std::weak_ptr<sf::Texture> texture)
{
	m_sprites.resize(m_vertexArray.getVertexCount());
	for (int i = 0; i < m_sprites.size(); ++i)
	{
		m_sprites[i].setTexture(*texture.lock());
		m_sprites[i].setPosition(m_vertexArray[i].position);
	}
}

void Path::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_sprites.size(); ++i)
		target.draw(m_sprites[i], states);
}


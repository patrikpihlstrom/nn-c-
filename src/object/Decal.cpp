#include "object/Decal.hpp"

Decal::Decal() :
	Object()
{
}

Decal::~Decal()
{
}

void Decal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

void Decal::rotate(const float& angle)
{
	m_shape.rotate(math::toDegrees<float>(angle));
}

void Decal::setColor(const sf::Color& color)
{
	m_shape.setFillColor(color);
}

bool Decal::isEntity() const
{
	return false;
}

bool Decal::isPlayerEntity() const
{
	return false;
}

bool Decal::isGameObject() const
{
	return false;
}

bool Decal::isLight() const
{
	return false;
}



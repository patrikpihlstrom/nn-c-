#include "light/Light.hpp"


Light::Light()
{
}

Light::~Light()
{
}

void Light::setTexture(const std::weak_ptr<sf::Texture> texture)
{
	m_texture = texture;
}


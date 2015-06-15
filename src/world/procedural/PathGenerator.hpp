#pragma once

#include <memory>

#include "world/Path.hpp"
#include "application/Math.hpp"


class PathGenerator
{
public:
	PathGenerator();
	~PathGenerator();

	void setTexture(const std::weak_ptr<sf::Texture> texture);

	Path generatePath(const sf::Vector2f& origin, const sf::Vector2f& destination, const uint8_t& resolution, const float& maxDeviation);

private:
	std::weak_ptr<sf::Texture> m_texture;

};


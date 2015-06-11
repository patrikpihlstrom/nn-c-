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

	Path generatePath(const sf::Vector2f& origin, const uint8_t& segmentCount, const sf::Vector2f& direction, const uint8_t& step, const float& maxDeviation);

private:
	std::weak_ptr<sf::Texture> m_texture;

};


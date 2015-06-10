#pragma once

#include "world/Path.hpp"
#include "application/Math.hpp"


class PathGenerator
{
public:
	PathGenerator();
	~PathGenerator();

	Path generatePath(const sf::Vector2f& origin, const uint8_t& segmentCount, const sf::Vector2f& direction, const uint8_t& step, const float& maxDeviation);

private:

};


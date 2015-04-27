#pragma once

#include <cassert>

#include "application/Math.hpp"


class RockGenerator
{
public:
	RockGenerator();
	~RockGenerator();

	std::vector<math::Polygon> getRock(const unsigned short& pointCount, const sf::Vector2<unsigned short>& size, const sf::Vector2f& position) const; // scale: 1:64 pixels, x = min y = max
};


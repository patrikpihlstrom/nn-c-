#include "world/procedural/PathGenerator.hpp"


PathGenerator::PathGenerator()
{
}

PathGenerator::~PathGenerator()
{
}

Path PathGenerator::generatePath(const sf::Vector2f& origin, const uint8_t& segmentCount, const sf::Vector2f& direction, const uint8_t& step, const float& maxDeviation)
{
	Path path;

	path.addPoint(origin);

	for (int i = 0; i < segmentCount; ++i)
	{
		auto point = sf::Vector2f(origin.x + (i + 1)*direction.x*step, origin.y + (i + 1)*direction.y*step);
		math::rotateVector<float>(i == 0 ? origin:path.getPoint(i - 1), point, (float)(rand()%(int)(maxDeviation*200))/100 - maxDeviation);
		path.addPoint(point);
	}

	return path;
}


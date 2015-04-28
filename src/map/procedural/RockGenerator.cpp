#include "RockGenerator.hpp"


RockGenerator::RockGenerator()
{
}

RockGenerator::~RockGenerator()
{
}

std::vector<math::Polygon> RockGenerator::getRock(const unsigned short& pointCount, const sf::Vector2<unsigned short>& size, const sf::Vector2f& position) const
{
	assert(pointCount > 2);

	std::vector<math::Polygon> polygons;
	polygons.reserve(pointCount + 1);

	math::Polygon polygon;

	for (unsigned short i = 0; i < pointCount + 1; ++i)
	{
		sf::Vector2f point = position;

		if (polygon.getPointCount() == 0)
			polygon.addPoint(point);
		
		float angle = i*((2*M_PI)/(pointCount));
		auto radius = size.x + rand() % size.y;

		polygon.addPoint(sf::Vector2f(std::cos(angle)*radius + position.x, std::sin(angle)*radius + position.y));
	}

	polygon.convexHull();
	polygon.constructEdges();
	polygons.push_back(polygon);

	return polygons;
}

